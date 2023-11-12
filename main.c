#include "main.h"

int main(void)
{
    uint8_t random_bytes[100];
    getrandom(&random_bytes, sizeof(random_bytes), 0);
    Object *objs[100];
    Object *obj = malloc(sizeof(Object) * 100);
    for (int i = 0; i < 100; i++)
    {
        assert(obj != NULL);
        (obj + i)->data = malloc(random_bytes[i] + 1);
        assert((obj + i)->data != NULL);
        strcpy((obj + i)->data, "H\n");
        (obj + i)->size = random_bytes[i] + 1;
        objs[i] = (obj + i);
    }

    printf("%p\n", objs[69]->data);
    void *new_addr = realloc(objs[69]->data, objs[69]->size + 16);
    objs[69]->data = new_addr;
    Object *o = (struct Object *)objs[69];
    printf("%p\n", o->data);

    // for (int i = 0; i < 100; i++)
    // {
    //     // swapping these results in a segfault
    //     free(objs[i]->data);
    //     free(objs[i]);
    // }
    return 0;
}