#include "main.h"

int main(void)
{
    uint16_t random_bytes[100];
    getrandom(&random_bytes, sizeof(random_bytes), 0);
    Object *objs[100];
    for (int i = 0; i < 100; i++)
    {
        // Object obj = {data : NULL, size : random_bytes[i]};
        // obj.data = malloc(obj.size);
        // objs[i] = &obj;

        Object *obj = malloc(sizeof(Object));
        obj->data = malloc(random_bytes[i]);
        obj->size = random_bytes[i];
        objs[i] = obj;
    }

    for (int i = 0; i < 100; i++)
    {
        printf("Size of Object %d is %ld \n", i, objs[i]->size);
    }
    return 0;
}