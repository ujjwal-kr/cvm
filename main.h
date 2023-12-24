#pragma once

// #include <stdio.h>
#include <inttypes.h>
#include <sys/random.h>
#include <string.h>
#include <assert.h>

int add(int a, int b);

typedef struct Object
{
    void *data;
    size_t size;
} Object;