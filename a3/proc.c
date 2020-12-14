#include <stdio.h> 

// how to read and print various things in /proc
// WARNING: This code is extremely old 
// AND it has not proven useful to past students. 
// -- Prof. Couch

/////////////////////////////////
// how to read /proc/*/stat
/////////////////////////////////

struct statStuff { 
    int pid;			// %d 
    char comm[256];		// %s
    char state;			// %c
    int ppid;			// %d
    int pgrp;			// %d
    int session;		// %d
    int tty_nr;			// %d
    int tpgid;			// %d
    unsigned long flags;	// %lu
    unsigned long minflt;	// %lu
    unsigned long cminflt;	// %lu
    unsigned long majflt;	// %lu
    unsigned long cmajflt;	// %lu
    unsigned long utime;	// %lu
    unsigned long stime; 	// %lu
    long cutime;		// %ld
    long cstime;		// %ld
    long priority;		// %ld
    long nice;			// %ld
    long num_threads;		// %ld
    long itrealvalue;		// %ld
    unsigned long starttime;	// %lu
    unsigned long vsize;	// %lu
    long rss;			// %ld
    unsigned long rlim;		// %lu
    unsigned long startcode;	// %lu
    unsigned long endcode;	// %lu
    unsigned long startstack;	// %lu
    unsigned long kstkesp;	// %lu
    unsigned long kstkeip;	// %lu
    unsigned long signal;	// %lu
    unsigned long blocked;	// %lu
    unsigned long sigignore;	// %lu
    unsigned long sigcatch;	// %lu
    unsigned long wchan;	// %lu
    unsigned long nswap;	// %lu
    unsigned long cnswap;	// %lu
    int exit_signal;		// %d
    int processor;		// %d
    unsigned long rt_priority;	// %lu 
    unsigned long policy;	// %lu 
    unsigned long long delayacct_blkio_ticks;	// %llu 
} ; 

static int readStat(int pid, struct statStuff *s) { 
    
    const char *format = "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %llu"; 

    char buf[256]; 
    FILE *proc; 
    sprintf(buf,"/proc/%d/stat",pid); 
    proc = fopen(buf,"r"); 
    if (proc) { 
        if (42==fscanf(proc, format, 
	    &s->pid,
	    s->comm,
	    &s->state,
	    &s->ppid,
	    &s->pgrp,
	    &s->session,
	    &s->tty_nr,
	    &s->tpgid,
	    &s->flags,
	    &s->minflt,
	    &s->cminflt,
	    &s->majflt,
	    &s->cmajflt,
	    &s->utime,
	    &s->stime,
	    &s->cutime,
	    &s->cstime,
	    &s->priority,
	    &s->nice,
	    &s->num_threads,
	    &s->itrealvalue,
	    &s->starttime,
	    &s->vsize,
	    &s->rss,
	    &s->rlim,
	    &s->startcode,
	    &s->endcode,
	    &s->startstack,
	    &s->kstkesp,
	    &s->kstkeip,
	    &s->signal,
	    &s->blocked,
	    &s->sigignore,
	    &s->sigcatch,
	    &s->wchan,
	    &s->nswap,
	    &s->cnswap,
	    &s->exit_signal,
	    &s->processor,
	    &s->rt_priority,
	    &s->policy,
	    &s->delayacct_blkio_ticks
	)) { 
	   fclose(proc); 
	   return 1; 
        } else { 
	   fclose(proc); 
	   return 0; 
        } 
     } else {  
	return 0; 
     } 
} 

static void printStat(FILE *out, struct statStuff *stuff) { 
    fprintf(out,"pid = %d\n", stuff->pid); 
    fprintf(out,"comm = %s\n", stuff->comm); 
    fprintf(out,"state = %c\n", stuff->state); 
    fprintf(out,"ppid = %d\n", stuff->ppid); 
    fprintf(out,"pgrp = %d\n", stuff->pgrp); 
    fprintf(out,"session = %d\n", stuff->session); 
    fprintf(out,"tty_nr = %d\n", stuff->tty_nr); 
    fprintf(out,"tpgid = %d\n", stuff->tpgid); 
    fprintf(out,"flags = %lu\n", stuff->flags); 
    fprintf(out,"minflt = %lu\n", stuff->minflt); 
    fprintf(out,"cminflt = %lu\n", stuff->cminflt); 
    fprintf(out,"majflt = %lu\n", stuff->majflt); 
    fprintf(out,"cmajflt = %lu\n", stuff->cmajflt); 
    fprintf(out,"utime = %lu\n", stuff->utime); 
    fprintf(out,"stime = %lu\n", stuff->stime); 
    fprintf(out,"cutime = %ld\n", stuff->cutime); 
    fprintf(out,"cstime = %ld\n", stuff->cstime); 
    fprintf(out,"priority = %ld\n", stuff->priority); 
    fprintf(out,"nice = %ld\n", stuff->nice); 
    fprintf(out,"num_threads = %ld\n", stuff->num_threads); 
    fprintf(out,"itrealvalue = %ld\n", stuff->itrealvalue); 
    fprintf(out,"starttime = %lu\n", stuff->starttime); 
    fprintf(out,"vsize = %lu\n", stuff->vsize); 
    fprintf(out,"rss = %ld\n", stuff->rss); 
    fprintf(out,"rlim = %lu\n", stuff->rlim); 
    fprintf(out,"startcode = %lu\n", stuff->startcode); 
    fprintf(out,"endcode = %lu\n", stuff->endcode); 
    fprintf(out,"startstack = %lu\n", stuff->startstack); 
    fprintf(out,"kstkesp = %lu\n", stuff->kstkesp); 
    fprintf(out,"kstkeip = %lu\n", stuff->kstkeip); 
    fprintf(out,"signal = %lu\n", stuff->signal); 
    fprintf(out,"blocked = %lu\n", stuff->blocked); 
    fprintf(out,"sigignore = %lu\n", stuff->sigignore); 
    fprintf(out,"sigcatch = %lu\n", stuff->sigcatch); 
    fprintf(out,"wchan = %lu\n", stuff->wchan); 
    fprintf(out,"nswap = %lu\n", stuff->nswap); 
    fprintf(out,"cnswap = %lu\n", stuff->cnswap); 
    fprintf(out,"exit_signal = %d\n", stuff->exit_signal); 
    fprintf(out,"processor = %d\n", stuff->processor); 
    fprintf(out,"rt_priority = %lu\n", stuff->rt_priority); 
    fprintf(out,"policy = %lu\n", stuff->policy); 
    fprintf(out,"delayacct_blkio_ticks = %llu\n", stuff->delayacct_blkio_ticks); 
} 

/////////////////////////////////
// how to read /proc/*/statm
/////////////////////////////////

struct statmStuff { 
    unsigned long size; 
    unsigned long resident; 
    unsigned long share; 
    unsigned long text; 
    unsigned long lib; 
    unsigned long data; 
    unsigned long dt; 
} ; 

static int readStatm(int pid, struct statmStuff *s) { 
    
    const char *format = "%lu %lu %lu %lu %lu %lu %ld"; 

    char buf[256]; 
    FILE *proc; 
    sprintf(buf,"/proc/%d/statm",pid); 
    proc = fopen(buf,"r"); 
    if (proc) { 
        if (7==fscanf(proc, format, 
	    &s->size,
	    &s->resident,
	    &s->share,
	    &s->text,
	    &s->lib,
	    &s->data,
	    &s->dt
	)) { 
	   fclose(proc); 
	   return 1; 
        } else { 
	   fclose(proc); 
	   return 0; 
        } 
     } else {  
	return 0; 
     } 
} 

static void printStatm(FILE *out, struct statmStuff *stuff) { 
    fprintf(out,"size = %lu\n", stuff->size); 
    fprintf(out,"resident = %lu\n", stuff->resident); 
    fprintf(out,"share = %lu\n", stuff->share); 
    fprintf(out,"text = %lu\n", stuff->text); 
    fprintf(out,"lib = %lu\n", stuff->lib); 
    fprintf(out,"data = %lu\n", stuff->data); 
    fprintf(out,"dt = %lu\n", stuff->dt); 
} 

/////////////////////////////////
// how to read /proc/*/status
/////////////////////////////////

struct statusStuff { 
    char Name[256];			// tcsh
    char State;				// S (sleeping)
    unsigned long SleepAVG;		//	98%
    unsigned long Tgid;			//	20616
    unsigned long Pid;			//	20616
    unsigned long PPid;			//	20612
    unsigned long TracerPid;		//	0
    unsigned long Uid[4];		//	418	418	418	418
    unsigned long Gid[4];		//	30	30	30	30
    unsigned long FDSize;		//	64
    unsigned long Groups[16];		//	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245 
    unsigned long VmPeak;		//	   64732 kB
    unsigned long VmSize;		//	   64700 kB
    unsigned long VmLck;		//	       0 kB
    unsigned long VmHWM;		//	    1756 kB
    unsigned long VmRSS;		//	    1756 kB
    unsigned long VmData;		//	    1112 kB
    unsigned long VmStk;		//	     348 kB
    unsigned long VmExe;		//	     320 kB
    unsigned long VmLib;		//	    1496 kB
    unsigned long VmPTE;		//	      68 kB
    unsigned long StaBrk;		//	0871a000 kB
    unsigned long Brk;			//	0879b000 kB
    unsigned long StaStk;		//	7fff6d0ccc70 kB
    unsigned long Threads;		//	1
    unsigned long SigQ[2];		//	1/16368
    unsigned long SigPnd;		//	0000000000000000
    unsigned long ShdPnd;		//	0000000000000000
    unsigned long SigBlk;		//	0000000000000002
    unsigned long SigIgn;		//	0000000000384004
    unsigned long SigCgt;		//	0000000009812003
    unsigned long CapInh;		//	0000000000000000
    unsigned long CapPrm;		//	0000000000000000
    unsigned long CapEff;		//	0000000000000000
    unsigned long Cpus_allowed[8];	//	00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff
    unsigned long Mems_allowed[2];	//	00000000,00000001
}; 

static int readStatus(int pid, struct statusStuff *s) { 
    int i; 
    char name[256]; 
    char buf[256]; 
    FILE *proc; 
    sprintf(name,"/proc/%d/status",pid); 
    proc = fopen(name,"r"); 
    if (proc) { 
	// Name:	tcsh
	fgets(buf,256,proc); sscanf(buf,"Name:\t%s",s->Name); 
	// State:	S (sleeping)
	fgets(buf,256,proc); sscanf(buf,"State:\t%c",&s->State); 
	// SleepAVG:	98%
	fgets(buf,256,proc); sscanf(buf,"SleepAVG:\t%lu",&s->SleepAVG); 
	// Tgid:	20616
	fgets(buf,256,proc); sscanf(buf,"Tgid:\t%lu",&s->Tgid); 
	// Pid:	20616
	fgets(buf,256,proc); sscanf(buf,"Pid:\t%lu",&s->Pid); 
	// PPid:	20612
	fgets(buf,256,proc); sscanf(buf,"PPid:\t%lu",&s->PPid); 
	// TracerPid:	0
	fgets(buf,256,proc); sscanf(buf,"TracerPid:\t%lu",&s->TracerPid); 
	// Uid:	418	418	418	418
	fgets(buf,256,proc); sscanf(buf,"Uid:\t%lu\t%lu\t%lu\t%lu",s->Uid,s->Uid+1, s->Uid+2, s->Uid+3); 
	// Gid:	30	30	30	30
	fgets(buf,256,proc); sscanf(buf,"Gid:\t%lu\t%lu\t%lu\t%lu",s->Gid,s->Gid+1, s->Gid+2, s->Gid+3); 
	// FDSize:	64
	fgets(buf,256,proc); sscanf(buf,"FDSize:\t%lu",&s->FDSize); 
	// Groups:	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245 
	fgets(buf,256,proc); 
	for (i=0; i<16; i++) s->Groups[i]=0; 
        i = sscanf(buf,"Groups:\t%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld",
	       s->Groups,
	       s->Groups+1, 
               s->Groups+2, 
               s->Groups+3, 
               s->Groups+4, 
               s->Groups+5, 
               s->Groups+6, 
               s->Groups+7, 
               s->Groups+8, 
               s->Groups+9, 
               s->Groups+10, 
               s->Groups+11, 
               s->Groups+12, 
               s->Groups+13, 
               s->Groups+14, 
	       s->Groups+15); 
	// VmPeak:	   64732 kB
	fgets(buf,256,proc); sscanf(buf,"VmPeak:\t%lu",&s->VmPeak); 
	// VmSize:	   64700 kB
	fgets(buf,256,proc); sscanf(buf,"VmSize:\t%lu",&s->VmSize); 
	// VmLck:	       0 kB
	fgets(buf,256,proc); sscanf(buf,"VmLck:\t%lu",&s->VmLck); 
	// VmHWM:	    1756 kB
	fgets(buf,256,proc); sscanf(buf,"VmHWM:\t%lu",&s->VmHWM); 
	// VmRSS:	    1756 kB
	fgets(buf,256,proc); sscanf(buf,"VmRSS:\t%lu",&s->VmRSS); 
	// VmData:	    1112 kB
	fgets(buf,256,proc); sscanf(buf,"VmData:\t%lu",&s->VmData); 
	// VmStk:	     348 kB
	fgets(buf,256,proc); sscanf(buf,"VmStk:\t%lu",&s->VmStk); 
	// VmExe:	     320 kB
	fgets(buf,256,proc); sscanf(buf,"VmExe:\t%lu",&s->VmExe); 
	// VmLib:	    1496 kB
	fgets(buf,256,proc); sscanf(buf,"VmLib:\t%lu",&s->VmLib); 
	// VmPTE:	      68 kB
	fgets(buf,256,proc); sscanf(buf,"VmPTE:\t%lu",&s->VmPTE); 
	// StaBrk:	0871a000 kB
	fgets(buf,256,proc); sscanf(buf,"StaBrk:\t%lx",&s->StaBrk); 
	// Brk:	0879b000 kB
	fgets(buf,256,proc); sscanf(buf,"Brk:\t%lx",&s->Brk); 
	// StaStk:	7fff6d0ccc70 kB
	fgets(buf,256,proc); sscanf(buf,"StaStk:\t%lx",&s->StaStk); 
	// Threads:	1
	fgets(buf,256,proc); sscanf(buf,"Threads:\t%lu",&s->Threads); 
	// SigQ:	1/16368
	fgets(buf,256,proc); sscanf(buf,"SigQ:\t%lu/%lu",s->SigQ,s->SigQ+1); 
	// SigPnd:	0000000000000000
	fgets(buf,256,proc); sscanf(buf,"SigPnd:\t%lx",&s->SigPnd); 
	// ShdPnd:	0000000000000000
	fgets(buf,256,proc); sscanf(buf,"ShdPnd:\t%lx",&s->ShdPnd); 
	// SigBlk:	0000000000000002
	fgets(buf,256,proc); sscanf(buf,"SigBlk:\t%lx",&s->SigBlk); 
	// SigIgn:	0000000000384004
	fgets(buf,256,proc); sscanf(buf,"SigIgn:\t%lx",&s->SigIgn); 
	// SigCgt:	0000000009812003
	fgets(buf,256,proc); sscanf(buf,"SigCgt:\t%lx",&s->SigCgt); 
	// CapInh:	0000000000000000
	fgets(buf,256,proc); sscanf(buf,"CapInh:\t%lx",&s->CapInh); 
	// CapPrm:	0000000000000000
	fgets(buf,256,proc); sscanf(buf,"CapPrm:\t%lx",&s->CapPrm); 
	// CapEff:	0000000000000000
	fgets(buf,256,proc); sscanf(buf,"CapEff:\t%lx",&s->CapEff); 
	// Cpus_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff
	fgets(buf,256,proc); 
	for (i=0; i<8; i++) s->Cpus_allowed[i]=0; 
        sscanf(buf,"Cpus_allowed:\t%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx",
	       s->Cpus_allowed,   s->Cpus_allowed+1, 
               s->Cpus_allowed+2, s->Cpus_allowed+3, 
               s->Cpus_allowed+4, s->Cpus_allowed+5, 
               s->Cpus_allowed+6, s->Cpus_allowed+7); 
	// Mems_allowed:	00000000,00000001
	fgets(buf,256,proc); sscanf(buf,"Mems_allowed:\t%lx,%lx",&(s->Mems_allowed[0]), &(s->Mems_allowed[1])); 
	fclose(proc); 
	return 1; 
    } else { 
	return 0; 
    } 
} 

static void printStatus(FILE *out, struct statusStuff *s) { 
    // int i; 
    // Name:	tcsh
    fprintf(out,"Name:\t%s\n",s->Name); 
    // State:	S (sleeping)
    fprintf(out,"State:\t%c\n",s->State); 
    // SleepAVG:	98%
    fprintf(out,"SleepAVG:\t%lu\n",s->SleepAVG); 
    // Tgid:	20616
    fprintf(out,"Tgid:\t%lu\n",s->Tgid); 
    // Pid:	20616
    fprintf(out,"Pid:\t%lu\n",s->Pid); 
    // PPid:	20612
    fprintf(out,"PPid:\t%lu\n",s->PPid); 
    // TracerPid:	0
    fprintf(out,"TracerPid:\t%lu\n",s->TracerPid); 
    // Uid:	418	418	418	418
    fprintf(out,"Uid:\t%lu\t%lu\t%lu\t%lu\n",s->Uid[0],s->Uid[1], s->Uid[2], s->Uid[3]); 
    // Gid:	30	30	30	30
    fprintf(out,"Gid:\t%lu\t%lu\t%lu\t%lu\n",s->Gid[0],s->Gid[1], s->Gid[2], s->Gid[3]); 
    // FDSize:	64
    fprintf(out,"FDSize:\t%lu\n",s->FDSize); 
    // Groups:	30 118 121 136 148 260 262 724 728 60045 60053 60072 600159 600217 600241 600245 
    fprintf(out,"Groups:\t%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n",
	   s->Groups[0],
	   s->Groups[1], 
	   s->Groups[2], 
	   s->Groups[3], 
	   s->Groups[4], 
	   s->Groups[5], 
	   s->Groups[6], 
	   s->Groups[7], 
	   s->Groups[8], 
	   s->Groups[9], 
	   s->Groups[10], 
	   s->Groups[11], 
	   s->Groups[12], 
	   s->Groups[13], 
	   s->Groups[14], 
	   s->Groups[15]); 
    // VmPeak:	   64732 kB
    fprintf(out,"VmPeak:\t%lu\n",s->VmPeak); 
    // VmSize:	   64700 kB
    fprintf(out,"VmSize:\t%lu\n",s->VmSize); 
    // VmLck:	       0 kB
    fprintf(out,"VmLck:\t%lu\n",s->VmLck); 
    // VmHWM:	    1756 kB
    fprintf(out,"VmHWM:\t%lu\n",s->VmHWM); 
    // VmRSS:	    1756 kB
    fprintf(out,"VmRSS:\t%lu\n",s->VmRSS); 
    // VmData:	    1112 kB
    fprintf(out,"VmData:\t%lu\n",s->VmData); 
    // VmStk:	     348 kB
    fprintf(out,"VmStk:\t%lu\n",s->VmStk); 
    // VmExe:	     320 kB
    fprintf(out,"VmExe:\t%lu\n",s->VmExe); 
    // VmLib:	    1496 kB
    fprintf(out,"VmLib:\t%lu\n",s->VmLib); 
    // VmPTE:	      68 kB
    fprintf(out,"VmPTE:\t%lu\n",s->VmPTE); 
    // StaBrk:	0871a000 kB
    fprintf(out,"StaBrk:\t%lx\n",s->StaBrk); 
    // Brk:	0879b000 kB
    fprintf(out,"Brk:\t%lx\n",s->Brk); 
    // StaStk:	7fff6d0ccc70 kB
    fprintf(out,"StaStk:\t%lx\n",s->StaStk); 
    // Threads:	1
    fprintf(out,"Threads:\t%lu\n",s->Threads); 
    // SigQ:	1/16368
    fprintf(out,"SigQ:\t%lu/%lu\n",s->SigQ[0],s->SigQ[1]); 
    // SigPnd:	0000000000000000
    fprintf(out,"SigPnd:\t%lx\n",s->SigPnd); 
    // ShdPnd:	0000000000000000
    fprintf(out,"ShdPnd:\t%lx\n",s->ShdPnd); 
    // SigBlk:	0000000000000002
    fprintf(out,"SigBlk:\t%lx\n",s->SigBlk); 
    // SigIgn:	0000000000384004
    fprintf(out,"SigIgn:\t%lx\n",s->SigIgn); 
    // SigCgt:	0000000009812003
    fprintf(out,"SigCgt:\t%lx\n",s->SigCgt); 
    // CapInh:	0000000000000000
    fprintf(out,"CapInh:\t%lx\n",s->CapInh); 
    // CapPrm:	0000000000000000
    fprintf(out,"CapPrm:\t%lx\n",s->CapPrm); 
    // CapEff:	0000000000000000
    fprintf(out,"CapEff:\t%lx\n",s->CapEff); 
    // Cpus_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,000000ff
    
    fprintf(out,"Cpus_allowed:\t%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx\n",
	   s->Cpus_allowed[0], s->Cpus_allowed[1], 
	   s->Cpus_allowed[2], s->Cpus_allowed[3], 
	   s->Cpus_allowed[4], s->Cpus_allowed[5], 
	   s->Cpus_allowed[6], s->Cpus_allowed[7]); 
    // Mems_allowed:	00000000,00000001
    fprintf(out,"Mems_allowed:\t%lx,%lx\n",s->Mems_allowed[0], s->Mems_allowed[1]); 
} 
