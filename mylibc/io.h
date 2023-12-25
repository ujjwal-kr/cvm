#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "mem.h"

void printf(const char *str, ...)
{
    int count = 0;
    if (count == 0)
    {
        write(1, str, strlen(str));
    }
}