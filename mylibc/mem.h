#include <stddef.h>
#include <sys/mman.h>

#define SEGMENT_SIZE 0x3e80
#define MAP_ANONYMOUS 0x20 // do not use a file

typedef struct
{
    size_t capacity;
    size_t size;
    size_t chunks;
    void *start;
} Segment;

Segment segments[10];
size_t segment_count = 0;
int segment_init = 1;

void new_segment()
{
    Segment segment;
    segment.capacity = SEGMENT_SIZE;
    segment.size = 0x0;
    segment.chunks = 0;
    segment.start = mmap(
        NULL,
        segment.capacity,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);
    segments[segment_count] = (Segment)segment;
    if (segment_init != 1)
    {
        segment_count++;
    }
}

// returns the latest segment
Segment *latest_segment(size_t size, char *segment_idx)
{
    size_t remaining_size = SEGMENT_SIZE - segments[segment_count].size;
    if (segment_count < 1 && segment_init == 1)
    {
        new_segment();
        segment_init = 0;
    }
    else if (remaining_size < size)
    {
        new_segment();
    }
    *segment_idx = (char)segment_count;
    return &segments[segment_count];
}

void get_chunk_props(void *ptr, char *size, char *free, char *index)
{
    char *s = ptr - sizeof(char);
    char *f = ptr - sizeof(char) * 2;
    char *i = ptr - sizeof(char) * 3;
    *size = (char)*s;
    *free = (char)*f;
    *index = (char)*i;
}

// returns pointer to free space in memory; if the chunk has the desired size
void *get_free_mem(size_t size)
{
    for (int i = 0; i < 10; i++)
    {
        Segment segment = segments[i];
        if (segment.size == 0)
        {
            return segment.start;
        }
        size_t offset = 0;
        for (int j = 0; j < segment.chunks; j++)
        {
            char chunk_size, free, index;
            get_chunk_props(segment.start + offset, &chunk_size, &free, &index);
            if (free == 1 && chunk_size <= size) {
                return segment.start + offset;
            }
            offset += chunk_size;
        }
    }
    return NULL;
}

// returns a pointer to the beginning of the chunk in segment
void *add_chunk(size_t size, Segment *segment, char segment_idx)
{
    void *start = segment->start + segment->size;
    char *s = start;
    s[0] = segment_idx;
    s[1] = 0; // free bit
    s[2] = size;
    segment->size += size + sizeof(char) * 3;
    segment->chunks += 1;
    return start + sizeof(char) * 3;
}

void *malloc(size_t size)
{
    if (size > SEGMENT_SIZE)
    {
        return NULL;
    }
    char segment_idx;
    Segment *latest = latest_segment(size, &segment_idx);
    return add_chunk(size, latest, segment_idx);
}

void free(void *ptr)
{
    char size, free, index;
    get_chunk_props(ptr, &size, &free, &index);
    if (free == 1)
        return;
    char *f = ptr - sizeof(char) * 2;
    *f = 1;
}

// void *calloc(size_t nmemb, size_t size);
// void *realloc(void *ptr, size_t size);