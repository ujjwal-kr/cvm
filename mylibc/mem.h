#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define SEGMENT_SIZE 0x4000

typedef struct
{
    size_t capacity;
    size_t size;
    void *start;
} Segment;

typedef struct
{
    size_t size;
    void *start;
} Chunk;

Segment segments[10];
size_t segment_count = 0;

void new_segment()
{
    Segment segment;
    segment.capacity = SEGMENT_SIZE;
    segment.size = 0x0;
    segment.start = mmap(
        NULL,
        segment.capacity,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);
    segments[segment_count] = (Segment)segment;
    if (segment_count != 0)
    {
        segment_count += 1;
    }
}

Segment *get_latest_segment()
{
    if (segment_count < 1)
    {
        new_segment();
    }
    return &segments[segment_count];
}

// returns a pointer to the beginning of the chunk in segment
void *add_chunk(size_t size, Segment *segment)
{
    void *start = segment[segment_count].start + segment[segment_count].size;
    segment[segment_count].size = segment[segment_count].size + size;
    return start;
}

void *malloc(size_t size)
{
    return add_chunk(size, get_latest_segment());
}
// void free(void *ptr);
// void *calloc(size_t nmemb, size_t size);
// void *realloc(void *ptr, size_t size);