#include <stdio.h>
#include <stdlib.h>

void main()
{
    fprintf(stderr, "running rand\n");
    int foo = rand();
    printf("rand() is %d\n", foo);
}
