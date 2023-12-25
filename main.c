#include "main.h"
#include "mylibc/mem.h"

int main(void)
{
    void *ok = malloc(1000);
    void *new_ok = realloc(ok, 1005); 
    free(ok);
}