// open a file 
#include <stdio.h> 
#define BUFLEN 1000
void main() { 
    FILE *fp = fopen("/etc/passwd", "r"); 
    char buf[BUFLEN]; 
    fgets(buf, BUFLEN, fp); 
    printf("oink! %s",buf); 
    fprintf(stderr, "oink! %s",buf); 
    fclose(fp); 
} 
