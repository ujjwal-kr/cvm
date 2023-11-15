#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

#define SEGMENT_SIZE 0x4000

typedef struct
{
    size_t total_size;
    size_t size;
    void *start;
} Segment;

typedef struct
{
    size_t size;
    void *start;
    void *prev_chunk;
} Chunk;

Segment segments[10];
size_t segment_count = 0;

void new_segment()
{
    Segment segment;
    segment.total_size = SEGMENT_SIZE;
    segment.size = 0x0;
    segment.start = mmap(
        NULL,
        segment.total_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);    
    segments[segment_count] = (Segment)segment;
    segment_count += 1;
}

// void *malloc(size_t size)
// void free(void *ptr);
// void *calloc(size_t nmemb, size_t size);
// void *realloc(void *ptr, size_t size);