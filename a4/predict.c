/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include "lru.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define MAX_PAGE_ALLOC 40
#define REALLOC_BASE 50
#define REALLOC_INTERVAL (REALLOC_BASE * 100) /* ticks */

#define HPLUS_ONE(_element) ((_element < (HSIZE - 1)) ? (_element + 1) : 0)
#define HMINUS_ONE(_element) ((_element == 0) ? (HSIZE - 1) : (_element - 1))

struct phist_record
{
    int page;
    int fault;
};

#define HSIZE (1 << 11)

struct phist
{
    struct phist_record records[HSIZE];
    int head;
    int tail;
};
/* ideas:
 *	-window
 *	-window w/ proactive eviction
 */
static void inc_head(struct phist *ph)
{
    int tmp;

    tmp = HPLUS_ONE(ph->head);
    if (tmp == ph->tail)
        ph->tail = HPLUS_ONE(ph->tail);

    ph->head = tmp;
}

void phist_init(struct phist *ph)
{
    ph->head = 0;
    ph->tail = 0;
}

void phist_push(struct phist *ph, const struct phist_record *ph_r)
{
    ph->records[ph->head] = *ph_r;
    inc_head(ph);
}

void phist_len(const struct phist *ph, int *len)
{
    int tmp;

    tmp = HPLUS_ONE(ph->head);
    if (tmp == ph->tail)
        *len = HSIZE;
    else
        *len = (ph->head - ph->tail) + 1;
}

void phist_at(const struct phist *ph, int t, struct phist_record *ph_r)
{
    int len, i, ptr;

    phist_len(ph, &len);
    assert(t >= 0 && t < len);

    ptr = ph->head;
    for (i = 0; i < t; i++)
    {
        ptr = HMINUS_ONE(ptr);
    }

    *ph_r = ph->records[ptr];
}

void phist_fault_sum(const struct phist *ph, int *sum)
{
    int ptr;

    *sum = 0;
    for (ptr = ph->tail; ptr != ph->head; ptr = HPLUS_ONE(ptr))
    {
        *sum += (ph->records[ptr].fault == 1);
    }
}

void phist_working_set(const struct phist *ph, int *pset, size_t psize)
{
    int ptr;
    unsigned int i;

    for (i = 0; i < psize; i++)
    {
        pset[i] = 0;
    }

    for (ptr = ph->tail; ptr != ph->head; ptr = HPLUS_ONE(ptr))
    {
        pset[ph->records[ptr].page] = 1;
    }
}

void phist_working_set_and_fault_sum(const struct phist *ph, int *pset, size_t psize, int *sum)
{
    int ptr;
    unsigned int i;

    for (i = 0; i < psize; i++)
    {
        pset[i] = 0;
    }
    *sum = 0;

    for (ptr = ph->tail; ptr != ph->head; ptr = HPLUS_ONE(ptr))
    {
        pset[ph->records[ptr].page] = 1;
        *sum += (ph->records[ptr].fault == 1);
    }
}

void phist_print_records(const struct phist *ph)
{
    int ptr;

    for (ptr = ph->tail; ptr != ph->head; ptr = HPLUS_ONE(ptr))
    {
        printf("%d ", ph->records[ptr].page);
    }
}
struct proc_fault_pair
{
    int proc;
    int faults;
};

static int cmp_pfp(const void *pfp1, const void *pfp2)
{
    struct proc_fault_pair *p1, *p2;
    p1 = (struct proc_fault_pair *)pfp1;
    p2 = (struct proc_fault_pair *)pfp2;

    if (p1->faults < p2->faults)
        return -1;
    else if (p1->faults == p2->faults)
        return 0;
    else
        return 1;
}

static void pred_pageit(Pentry q[MAXPROCESSES], uint32_t tick)
{
    //static int last_realloc = 0;
    int proc, page, state, evicted, i;
    struct phist_record ph_r;
    int unsat[MAXPROCESSES]; /* if unsat[proc] >= 0, that proc wants page unsat[proc] */
    int amt_unsat = 0;
    int allocated[MAXPROCESSES];
    int faults[MAXPROCESSES];
    int pset_sizes[MAXPROCESSES];
    int pages_freed = 0;
    int free_phys = 100;

    for (i = 0; i < MAXPROCESSES; i++)
        unsat[i] = -1;

    /*if(last_realloc == 0)
		last_realloc = 1;*/

    for (proc = 0; proc < MAXPROCESSES; proc++)
    {
        if (!q[proc].active) /* done if its not active */
            continue;

        allocated[proc] = pages_alloc(q, proc);
        free_phys -= allocated[proc]; /* anything thats already allocated isnt free */

        if (proc_susp[proc] > 0)
            continue;

        /*if(tick >= 150000) {

			fgetc(stdin);
		}*/

        page = q[proc].pc / PAGESIZE;
        /* note this time for future eviction decisions */
        timestamps[proc][page] = tick;

        /*if(tick - last_realloc >= REALLOC_INTERVAL ) {
			realloc_page_limits(q);
			last_realloc = tick;
		}*/

        ph_r.page = page;
        /* done if the page is already in memory */
        if (q[proc].pages[page])
        {
            ph_r.fault = 0;
            phist_push(&phist_arr[proc], &ph_r);
            continue;
        }

        ph_r.fault = 1;
        phist_push(&phist_arr[proc], &ph_r);

        phist_working_set_and_fault_sum(&phist_arr[proc], proc_pset[proc], MAXPROCESSES, &faults[proc]);

        pset_sizes[proc] = 0;
        for (i = 0; i < MAXPROCESSES; i++)
        {
            if (proc_pset[proc][i] == 1)
                pset_sizes[proc]++;
        }
        /* first: if there are processes out there with MORE
		 * allocated than is in their working set, they lose
		 * those pages
		 */
        if (pset_sizes[proc] < allocated[proc])
            for (i = 0; i < MAXPROCPAGES; i++)
                if (q[proc].pages[i] && proc_pset[proc][i] == 0)
                {
                    pageout(proc, i);
                    pages_freed++; /* dont ++ free_phys cause these need 100 ticks to actually be available */
                }

        //allocated[proc] = pages_alloc(q, proc);

        //if(tick >= 150000 ) printf("(%d) (%d/%d) %d: p%d => %d > %d \n", tick, faults[proc], HSIZE, proc, page, pset_sizes[proc], allocated[proc]);
        /* the page is not in memory.
		 * if pagein give 1 the page is either 
		 * on its way already or we just got it
		 * started, so we are done with this process
		 */
        state = SWAPFAIL; /* if we are at max alloc, the state is swap fail */
        //if( (allocated[proc] <= pg_alloc[proc]) && pagein(proc, page, &state) )
        if (pagein(proc, page, &state))
        {
            proc_last_pagein[proc] = tick;
            free_phys--; /* if its on its way, its not available for use */
            continue;
        }

        /* either the page is swapping out or 
		 * there are no free physical pages
		 */
        if (state == SWAPOUT)
            continue; /* just have to wait... */

        /* there are no free physical pages */
        if (allocated[proc] < 1 && (tick - proc_last_unsat[proc]) < 100)
            continue; /* must have at least one page to evict and we dont evict more than once per 100 ticks */

        unsat[proc] = page;
        amt_unsat++;
        proc_last_unsat[proc] = tick;
    }

    assert(free_phys >= 0);

    /* the idea here is to free as many pages as there were unsatisfied processes
	 * which may require suspending one or more processes */
    if (amt_unsat > 0)
    {
        /* get the working sets and page faults for all the 
		 * active processes so we know whats up */
        /*for(proc = 0; proc < MAXPROCESSES; proc++) {
			if(!q[proc].active)
				continue;
				
			
		}*/

        if (pages_freed >= amt_unsat)
            return;

        struct proc_fault_pair thrash_list[MAXPROCESSES];
        for (i = 0; i < MAXPROCESSES; i++)
        {
            thrash_list[i].proc = i;
            thrash_list[i].faults = ((q[i].active && (proc_susp[i] == 0)) ? faults[i] : -1);
        } /* ordered by proc */

        /* now reorder by faults */
        qsort(thrash_list, MAXPROCESSES, sizeof(struct proc_fault_pair), cmp_pfp);

        int k;
        for (k = 0; k < MAXPROCESSES; k++)
        {
            if ((tick - proc_last_pagein[thrash_list[MAXPROCESSES - 1 - k].proc]) > 100 && thrash_list[MAXPROCESSES - 1 - k].faults >= 2048 && allocated[thrash_list[MAXPROCESSES - 1 - k].proc] > 0)
            {
                for (i = 0; i < MAXPROCPAGES; i++)
                    if (q[thrash_list[MAXPROCESSES - 1 - k].proc].pages[i])
                    {
                        pageout(thrash_list[MAXPROCESSES - 1 - k].proc, i);
                        pages_freed++;
                    }

                if (unsat[thrash_list[MAXPROCESSES - 1 - k].proc] == 1)
                    pages_freed++; /* dont worry about satisfying this process */

                //printf("%d: (need %d pages) suspend thrasher w/ %d pages: %d = %d\n", tick, amt_unsat, allocated[thrash_list[MAXPROCESSES-1-k].proc], thrash_list[MAXPROCESSES-1-k].proc, thrash_list[MAXPROCESSES-1-k].faults);
                fflush(stdout);
                proc_susp[thrash_list[MAXPROCESSES - 1 - k].proc] = tick;
            }

            if (pages_freed >= amt_unsat)
                return;
        }

        /* free a page from each unsatisfied process until
		 * we have free at least amt_unsat pages */
        for (proc = 0; proc < MAXPROCESSES; proc++)
        {
            if (unsat[proc] < 0)
                continue;

            if (allocated[proc] < 1)
                continue;

            //if(pset_size > allocated)
            //if(faults < 1000)
            //if(pset_sizes[proc] > pages_alloc(q, proc) ) printf("(%d) (%d/%d) %d: p%d => %d > %d \n", tick, faults[proc], HSIZE, proc, unsat[proc], pset_sizes[proc], pages_alloc(q, proc));

            lru_page(q, proc, tick, &evicted);
            if (!pageout(proc, evicted))
            {
                endit();
            }
            pages_freed++;

            if (pages_freed >= amt_unsat)
                break;
        }
    } /* amt_unsat > 0 */
    else if (free_phys > 0)
    {
        /*printf("%d: %d FREE... susp: ", tick, free_phys);
		for(i = 0; i < MAXPROCESSES; i++)
			if(proc_susp[i] > 0)
				printf("%d, ", i);
		fputc('\n', stdout);*/

        /* unsuspend a process which has less or equal to 
		 * the number of free pages in its working set  */
        for (proc = 0; proc < MAXPROCESSES; proc++)
        {
            int wset_size = 0;
            if (proc_susp[proc] == 0)
                continue;

            if (tick - proc_susp[proc] < 500)
                continue;

            if (free_phys < 1)
                break;

            for (i = 0; i < MAXPROCPAGES; i++)
            {
                wset_size += (proc_pset[proc][i] != 0);
            }

            if (wset_size > free_phys)
                continue;

            //printf("%d: unsusp %d with %d pages (%d free)\n", tick, proc, wset_size, free_phys);
            proc_susp[proc] = 0;
            for (i = 0; i < MAXPROCPAGES; i++)
            {
                if (proc_pset[proc][i])
                {
                    if (!pagein(proc, i, &state))
                        free_phys = 0;

                    free_phys--;
                }
            }
        }
    }
}

void pageit(Pentry q[MAXPROCESSES])
{
    static uint32_t tick = 0; // artificial time

    /* tick starts at 1, so 0 means this is the first run
	 * or an overflow. either way, reset timestamps.
	 */
    if (tick < 1)
    {
        int i;

        timestamps_init();
        for (i = 0; i < MAXPROCESSES; i++)
        {
            phist_init(&phist_arr[i]);
            pg_alloc[i] = 20;
            proc_faults[i] = 0;
            proc_susp[i] = 0;
            proc_last_evict[i] = 0;
            proc_last_unsat[i] = 0;
            proc_last_pagein[i] = 0;
        }

        tick = 1;
    }

    pred_pageit(q, tick);

    /* advance time for next pageit iteration */
    tick++;
}