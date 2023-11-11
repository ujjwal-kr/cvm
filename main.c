#include "main.h"

int main(void)
{
    Object obj = {data : NULL, size : 69};
    obj.data = malloc(obj.size);
    obj.data = "Hello world\n";
    printf("%s", (char*) obj.data);
    return 0;
}