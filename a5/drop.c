#include <stdio.h> 
#include <unistd.h> 

/* 
 * if you are root, this forces a clear for the whole cache
 * sync writes out all dirty pages and marks them clean
 * writing 3 to /proc/sys/vm/drop_caches drops all clean pages
 */ 

int main() 
{ 
    sync(); sync(); 
    FILE *f = fopen("/proc/sys/vm/drop_caches", "w"); 
    fprintf(f, "3\n"); 
    printf("requested cache drop\n"); 
    return 0;
} 
