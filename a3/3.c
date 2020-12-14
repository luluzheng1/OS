// a very friendly and control-C sensitive fork botch.
#include <stdio.h>
#include <unistd.h>

void main()
{
    if (fork())
    {
        sleep(5);
        printf("oink!\n");
        fprintf(stderr, "oink!\n");
    }
}
