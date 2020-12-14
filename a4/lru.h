#ifndef OSPA4_LRU_H
#define OSPA4_LRU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "/comp/111/assignments/a4/t4.h"
#include "util.h"

static uint32_t timestamps[MAXPROCESSES][MAXPROCPAGES];

static void timestamps_init()
{
    int proc, page;

    for (proc = 0; proc < MAXPROCESSES; proc++)
        for (page = 0; page < MAXPROCPAGES; page++)
            timestamps[proc][page] = 0;
}

static void lru_page(Pentry q[MAXPROCESSES], int proc, uint32_t tick, int *evictee)
{
    int page;
    uint32_t t;

    *evictee = -1;
    /* want to do better than or equal to
	 * a page that was referenced just 
	 * now. adding 1 to tick should
	 * ensure this function always
	 * returns a page
	 */
    t = tick + 1;

    for (page = 0; page < MAXPROCPAGES; page++)
    {
        if (!q[proc].pages[page]) /* cant evict this */
            continue;

        if (timestamps[proc][page] < t)
        {
            t = timestamps[proc][page];
            *evictee = page;

            if (t <= 1) /* cant do any better than that! */
                break;
        }
    }

    if (*evictee < 0)
    {
        printf("page for process %d w/ %u active pages not found with age < %u\n", proc, (unsigned int)pages_alloc(q, proc), tick);
        fflush(stdout);
        endit();
    }
}

#if 0
typedef struct lru_record {
	uint32_t t;
	int proc;
	int page;
} lru_record_t;

void lru_page_global(Pentry q[MAXPROCESSES], uint32_t tick, lru_record_t *evictee) {
	int proc, page;
	
	evictee->proc = -1;
	evictee->page = -1;
	/* want to do better than or equal to
	 * a page that was referenced just 
	 * now. adding 1 to tick should
	 * ensure this function always
	 * returns a page
	 */
	evictee->t = tick+1;  
	for(proc = 0; proc < MAXPROCESSES; proc++) {
		if(q[proc].asdfasdf < 1)
			continue;

		for(page = 0; page < MAXPROCPAGES; page++) {
			if(!q[proc].pages[page])
				continue;

			if(timestamps[proc][page] < evictee->t) {
				evictee->t = timestamps[proc][page];
				evictee->proc = proc;
				evictee->page = page;

				if(evictee->t <= 1) /* cant do any better than that! */
					goto done;
			}
		}			
	}

done:
	endit();
}
#endif /* comment */

#endif /* OSPA4_LRU_H */