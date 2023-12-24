#include <stddef.h>
#include <sys/mman.h>

#define MAX_SEGMENTS 0xa
#define SEGMENT_SIZE 0x3e80
#define MAP_ANONYMOUS 0x20 // do not use a file

typedef struct
{
    size_t capacity;
    size_t size;
    size_t chunks;
    void *start;
} Segment;

typedef struct
{
    size_t size;
    char free;
    char index;
} Chunk;

Segment segments[MAX_SEGMENTS];
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
    if (remaining_size < size)
    {
        new_segment();
    }
    *segment_idx = (char)segment_count;
    return &segments[segment_count];
}

void get_chunk_props(void *ptr, size_t *size, char *free, char *index)
{
    void *chunk = ptr - sizeof(Chunk);
    Chunk *c = (Chunk *)chunk;
    *size = c->size;
    *free = c->free;
    *index = c->index;
}

void scan_chunk_props(void *ptr, size_t *size, char *free, char *index)
{
    void *chunk = ptr;
    Chunk *c = (Chunk *)chunk;
    *size = c->size;
    *free = c->free;
    *index = c->index;
}

// returns pointer to free space in memory; if the chunk has the desired size
void *get_free_mem(size_t size)
{
    for (int i = 0; i < MAX_SEGMENTS; i++)
    {
        Segment segment = segments[i];
        if (segment.size == 0)
        {
            return segment.start + sizeof(Chunk);
        }
        size_t offset = 0;
        for (int j = 0; j < segment.chunks; j++)
        {
            size_t chunk_size;
            char free, index;
            if (j == 0) {
                scan_chunk_props(segment.start, &chunk_size, &free, &index);
                if (free == 1 && chunk_size <= size)
                {
                    return segment.start + sizeof(Chunk);
                }
            } else {
                get_chunk_props(segment.start + offset, &chunk_size, &free, &index);
            }
            if (free == 1 && chunk_size <= size)
            {
                return segment.start + offset;
            }
            offset += chunk_size + sizeof(Chunk);
        }
    }
    return NULL;
}

// returns a pointer to the beginning of the chunk in segment
void *add_chunk(size_t size, Segment *segment, char segment_idx)
{
    void *start = segment->start + segment->size;
    Chunk *chunk = (Chunk *)start;

    chunk->size = size;
    chunk->index = segment_idx;
    chunk->free = (char)0;

    segment->size += size + sizeof(Chunk);
    segment->chunks += 1;
    return start + sizeof(Chunk);
}

void *malloc(size_t size)
{
    if (size > SEGMENT_SIZE)
    {
        return NULL;
    }
    if (segment_count < 1 && segment_init == 1)
    {
        new_segment();
        segment_init = 0;
    }
    else
    {
        void *free_chunk = get_free_mem(size);
        if (free_chunk != NULL)
        {
            return free_chunk;
        }
    }
    char segment_idx;
    Segment *latest = latest_segment(size, &segment_idx);
    return add_chunk(size, latest, segment_idx);
}

void free(void *ptr)
{
    size_t size;
    char free, index;
    get_chunk_props(ptr, &size, &free, &index);
    if (free == 1)
        return;
    void *chunk = ptr - sizeof(Chunk);
    Chunk *c = (Chunk *)chunk;
    c->free = 1;
}

// void *calloc(size_t nmemb, size_t size);
// void *realloc(void *ptr, size_t size);
