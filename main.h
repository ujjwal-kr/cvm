#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int add(int a, int b);

typedef struct Object
{
    void *data;
    size_t size;
} Object;