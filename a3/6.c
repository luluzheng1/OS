#include <stdio.h> 

// this should not be allowed. 
int foo[1000000]; // 8 mb; 
void main() { 
    printf("kilroy was here\n"); 
    printf("oink!\n"); 
    fprintf(stderr,"oink!\n"); 
} 
