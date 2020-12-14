// blow the stack with a bounded but intentionally antisocial recursion.
#include <stdio.h>

void oink(int i)
{
    int garbage[1024]; // push this onto the stack.
#ifdef DEBUG
    fprintf(stderr, "allocated %d bytes of stack memory\n", 1024 * sizeof(int));
#endif /* DEBUG */
    if (i > 0)
        oink(i - 1);
}

void main()
{
    oink(1200);
    printf("oink!\n");
    fprintf(stderr, "oink!\n");
}
