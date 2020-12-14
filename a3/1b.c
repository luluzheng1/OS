// blow the stack with a really large allocation.
#include <stdio.h>

void oink()
{
    int garbage[1200 * 1024]; // push this onto the stack.
    printf("oink!\n");
    fprintf(stderr, "oink!\n");
}

void main()
{
    oink();
}
