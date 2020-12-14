// open a file
#include <stdio.h>
#include <unistd.h>
#define BUFLEN 1000
void main()
{
    FILE *fp = fopen("/etc/passwd", "r");
    sleep(1);
    char buf[BUFLEN];
    while (!feof(fp))
    {
        fgets(buf, BUFLEN, fp);
        printf("%s", buf);
    }
    printf("oink!\n");
    fprintf(stderr, "oink!\n");
}
