#ifndef OSPA4_UTIL_H
#define OSPA4_UTIL_H

extern void endit();

static size_t pages_alloc(Pentry q[MAXPROCESSES], int proc)
{
    int page;
    size_t amt = 0;

    for (page = 0; page < MAXPROCPAGES; page++)
    {
        if (q[proc].pages[page])
            amt++;
    }

    return amt;
}

#if 0
static size_t ppages_alloc(Pentry q[MAXPROCESSES]) {
	int proc;
	size_t amt = 0;
	
	for(proc = 0; proc < MAXPROCESSES; proc++) {
		amt += pages_alloc(q, proc);
		if(amt == PHYSICALPAGES)
			break;
	}

	return amt;
}
#endif

#endif /* OSPA4_UTIL_H */