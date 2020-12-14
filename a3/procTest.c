#include "/comp/111/a/a3/proc.c"

static void catStat(FILE *out, int pid, const char *file) { 
    char name[256]; 
    char buffer[256]; 
    FILE *p; 
    sprintf(name,"/proc/%d/%s",pid,file); 
    p = fopen(name,"r"); 
    if (p) { 
        fprintf(out,"Contents of %s:\n",name); 
	while (fgets(buffer,256,p)) { 
	    fprintf(out,buffer); 
        } 
	fclose(p); 
    } else { 
	fprintf(out, "can't open %s\n",name); 
    } 
} 

void main()
{ 
   int ppid = getppid(); 
   printf("my parent id is %d\n",ppid); 
   struct statStuff stat; 
   struct statmStuff statm; 
   struct statusStuff status; 
   if (readStat(ppid,&stat)) { 
	printf("====================\n"); 
	printf("stat is\n"); 
	printf("====================\n"); 
	printStat(stdout,&stat); 
	printf("====================\n"); 
	printf("original file is\n"); 
	printf("====================\n"); 
	catStat(stdout,ppid,"stat"); 
   } else { 
	printf("====================\n"); 
	printf("didn't get stat\n"); 
	printf("====================\n"); 
   } 
   if (readStatm(ppid,&statm)) { 
	printf("====================\n"); 
	printf("statm is\n"); 
	printf("====================\n"); 
	printStatm(stdout,&statm); 
	printf("====================\n"); 
	printf("original file is\n"); 
	printf("====================\n"); 
	catStat(stdout,ppid,"statm"); 
   } else { 
	printf("====================\n"); 
	printf("didn't get statm\n"); 
	printf("====================\n"); 
   } 
   if (readStatus(ppid,&status)) { 
	printf("====================\n"); 
	printf("status is\n"); 
	printf("====================\n"); 
	printStatus(stdout,&status); 
	printf("====================\n"); 
	printf("original file is\n"); 
	printf("====================\n"); 
	catStat(stdout,ppid,"status"); 
   } else { 
	printf("====================\n"); 
	printf("didn't get status\n"); 
	printf("====================\n"); 
   } 
} 
