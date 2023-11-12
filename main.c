#include "main.h"

int main(void)
{
    uint8_t random_bytes[100];
    getrandom(&random_bytes, sizeof(random_bytes), 0);
    Object *objs[100];
    for (int i = 0; i < 100; i++)
    {
        // Object obj = {data : NULL, size : random_bytes[i]};
        // obj.data = malloc(obj.size);
        // objs[i] = &obj;

        Object *obj = malloc(sizeof(Object));
        obj->data = malloc(random_bytes[i] + 1);
        strcpy(obj->data, "H\n");
        obj->size = random_bytes[i] + 1;
        objs[i] = obj;
    }

    printf("%p\n", objs[69]->data);
    void *new_addr = realloc(objs[69]->data, objs[69]->size + 1);
    objs[69]->data = new_addr;
    printf("%p\n", objs[69]->data);
    return 0;
}