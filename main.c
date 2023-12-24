#include "main.h"
#include "mylibc/mem.h"
// #include <stdlib.h>

int main(void)
{
    uint8_t random_bytes[10];
    getrandom(&random_bytes, sizeof(random_bytes), 0);
    Object *objs[10];
    Object *obj = malloc(sizeof(Object) * 10);
    for (int i = 0; i < 10; i++)
    {
        assert(obj != NULL);
        (obj + i)->data = malloc(random_bytes[i] + 1);
        assert((obj + i)->data != NULL);
        strcpy((obj + i)->data, "H\n");
        (obj + i)->size = random_bytes[i] + 1;
        objs[i] = (obj + i);
    }

    for (int i = 0; i < 10; i++)
    {
        printf("Pointer to data %d: %p and String: %s", i, (obj + i)->data, (char *)(obj + i)->data);
    }

    printf("%p\n", objs[6]->data);
    // void *new_addr = realloc(objs[69]->data, objs[69]->size + 16);
    // objs[69]->data = new_addr;
    // Object *o = (struct Object *)objs[69];
    // printf("%p\n", o->data);

    for (int i = 0; i < 10; i++)
    {
        free((obj + i)->data);
    }
    free(obj);
    return 0;

    // Object *obj = malloc(500);
    // Object *obj2 = malloc(1000);
    // free(obj);
    // free(obj2);
    // return 0;
}