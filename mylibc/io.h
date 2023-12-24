#include <unistd.h>
#include <string.h>
#include <stdarg.h>

void printf(const char *str, ...) {
    write(1, str, strlen(str));
}