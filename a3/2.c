// try to take over all memory and allocate it on your heap
#include <stdio.h>
#include <malloc.h>

void main()
{
    int i;
    for (i = 0; i < 1200; i++)
    {
        int *p = (int *)malloc(1024 * sizeof(int));
    }
    printf("oink!\n");
    fprintf(stderr, "oink!\n");
}
