#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#include "/comp/111/assignments/a4/t4.h"

/**********************************************************************
 * William Richard
 * Comp 111
 * Assignment 5
 *
 * This pager has 2 things that are necessary to it's function
 * The first are the ProcMem and PageMem structs
 * The PageMem struct serves as a pointer from a given page
 * to all of the pages that come after it (that we have seen)
 * For example, if we have seen in the past that a process
 * wants page 2 or 3 after page 1, when it is at page 1 we
 * know to also try to load pages 2 and 3.
 * The ProcMem struct stores a PageMem struct for each page
 * that each process may have.
 * The updateProcMemory and addPgMemoryEntry functions
 * serve to update the array of ProcMems (one for each process)
 *
 * Once the updateProcMemory function has been called, we then
 * use that information to categorize pages into 3 groups.
 * 1) Active Pages - the pages that the processes are currently at
 * 2) Coming Up Pages - the pages that the ProcMem tells us are
 *                      likely to appear after the current page for
 *                      for each process
 * 3) Available Pages - These pages are allocated, but neither
 *                      Active nor Coming up. Therefore, they are
 *                      free to be flushed.
 * The PPP struct (stading for Process Page Pair) is used to store
 * the process and page numeber in each of these 3 categories.
 * This is better than doing a double array since, emperically, there
 * are never more than 20 active pages (obviously) and 25 coming up
 * pages.  It is much faster to travese an array of length 25 than
 * a bit array of 20*20 = 400
 *
 * The advantage of this approach is that it can adapt to any
 * looping process, regardless of structure, reasonably efficiently.
 * Granted, I could have tuned things a bit more accurately by using
 * the added knowledge of the structure of the various processes
 * but I feel this solution has a nice mix both of efficiency and
 * adaptablility.
 * In my tests, it results in a ratio block/compute between .065 and .09
 ************************************************************************/

//when we record which pages we want to put into memory, or which pages are available to be flushed
//we use this object for convince
//easier than having a double array, uses less memory, and faster to traverse
typedef struct _ts_proc_page_pair
{
    int proc;
    int page;
} PPP;

//this keeps track of which pages a process visits
//after visiting a certain page
typedef struct _ts_page_memory
{
    int size; //how many pages in the array
    int nextPage[MAXPROCPAGES];
} PageMem;

//this stores all of the PageMem objects
//so we can predict where a process may go next, given where it is
typedef struct _ts_process_memory
{
    int lastSeenAt;                    //need to keep track of where we last saw a process so that we can update that page memory listing
                                       //when the process moves to a new page
    PageMem procHistory[MAXPROCPAGES]; //keep track of what pages lead to eachother for all pages in each process
} ProcMem;

// these are globals that only the subroutine can see
//keep track of how we have seen each process act in the past
static ProcMem history[MAXPROCESSES];

void addPgMemoryEntry(PageMem *mem, int entryToAdd, int proc, int oldpage)
{
    //look through the current entries to see if we need to add the new one
    int i;
    for (i = 0; i < mem->size; ++i)
    {
        if (mem->nextPage[i] == entryToAdd)
        {
            return;
        }
    }
    //add the entry
    mem->nextPage[mem->size] = entryToAdd;
    ++(mem->size);
}

void updateProcMemory(Pentry *q)
{
    int proc, curPage, oldPage;
    for (proc = 0; proc < MAXPROCESSES; ++proc)
    {
        curPage = q[proc].pc / PAGESIZE;
        oldPage = history[proc].lastSeenAt;
        if (oldPage != curPage)
        {
            //record that the old page lead to the current page
            addPgMemoryEntry(&(history[proc].procHistory[oldPage]), curPage, proc, oldPage);
        }
        history[proc].lastSeenAt = curPage;
    }
}

void smartPageIn(PPP *active, int numActive, PPP *comingUp, int numComingUp, PPP *available, int numAvailable, Pentry *q)
{
    int i;
    PPP cur;
    int used = 0;

    //try to pagein the active ones
    for (i = 0; i < numActive; ++i)
    {
        cur = active[i];
        if (!q[cur.proc].pages[cur.page])
        {
            if (!pagein(cur.proc, cur.page))
            {
                //if we can't pagein any more pages, and we don't have any more available to pageout, then break
                if (used >= numAvailable)
                {
                    break;
                }
                pageout(available[used].proc, available[used].page);
                ++used;
            }
        }
    }

    for (i = 0; i < numComingUp; ++i)
    {
        cur = comingUp[i];
        if (!q[cur.proc].pages[cur.page])
        {
            if (!pagein(cur.proc, cur.page))
            {
                if (used >= numAvailable)
                {
                    //same deal as above - nothing else to do now
                    //not as crutial, since these pages aren't needed yet, and may never be needed
                    break;
                }
                pageout(available[used].proc, available[used].page);
                ++used;
            }
        }
    }
}

// simple algorithm does one job at a time
// with miserable parallelism
// this is called BY ME every time something interesting occurs.
//   q: state of every process.

void pageit(Pentry q[MAXPROCESSES])
{
    // these are regular dynamic variables on stack
    int proc, pc, page, nextPageIndex, pageLoop;

    static int firstTime = 1;
    if (firstTime)
    {
        //set initial values
        //namely, setting array sizes for the page histories to 0
        for (proc = 0; proc < MAXPROCESSES; ++proc)
        {
            for (page = 0; page < MAXPROCPAGES; ++page)
            {
                history[proc].procHistory[page].size = 0;
            }
        }
        firstTime = 0;
    }

    //update the process memory
    updateProcMemory(q);

    //figure out which pages are really needed
    PPP activePages[MAXPROCESSES];
    int numActive = 0;

    //which would be nice to have
    PPP comingUp[MAXPROCESSES * MAXPROCPAGES];
    int numComingUp = 0;

    //and which are available to be flushed
    PPP available[MAXPROCESSES * MAXPROCPAGES];
    int numAvailable = 0;

    PageMem pageHistory;

    //identify the pages in each of those arrays
    for (proc = 0; proc < MAXPROCESSES; proc++)
    {
        if (q[proc].active)
        {
            pc = q[proc].pc;      // program counter for process
            page = pc / PAGESIZE; // page the program counter needs

            //update our list of active pages
            activePages[numActive].proc = proc;
            activePages[numActive].page = page;
            ++numActive;

            //next, try to page in any of the pages it might use next
            pageHistory = history[proc].procHistory[page];
            for (nextPageIndex = 0; nextPageIndex < pageHistory.size; ++nextPageIndex)
            {
                comingUp[numComingUp].proc = proc;
                comingUp[numComingUp].page = pageHistory.nextPage[nextPageIndex];
                ++numComingUp;
            }

            //finally, look for avaiable pages
            //make sure they aren't active or coming up
            for (pageLoop = 0; pageLoop < q[proc].npages; ++pageLoop)
            {
                if (q[proc].pages[pageLoop])
                {
                    //check active
                    int activeIndex, comingUpIndex;
                    int canUse = 1;
                    for (activeIndex = numActive - 1; activeIndex >= 0 && canUse; --activeIndex)
                    {
                        if (activePages[activeIndex].proc == proc)
                        {
                            if (activePages[activeIndex].page == pageLoop)
                            {
                                canUse = 0;
                            }
                        }
                        else
                        {
                            //since this proc's pages were added last, when we reach a proc that is not the current proc
                            //we can break
                            break;
                        }
                    }

                    for (comingUpIndex = numComingUp - 1; comingUpIndex >= 0 && canUse; --comingUpIndex)
                    {
                        if (comingUp[comingUpIndex].proc == proc)
                        {
                            if (comingUp[comingUpIndex].page == pageLoop)
                            {
                                canUse = 0;
                            }
                        }
                        else
                        {
                            //again, since this proc's information was added at the end, we can break out
                            //when we reach the information about another process
                            break;
                        }
                    }
                    //if it is not a page we want in memory, we can put it in the list for removal
                    if (canUse)
                    {
                        available[numAvailable].proc = proc;
                        available[numAvailable].page = pageLoop;
                        ++numAvailable;
                    }
                }
            }
        }
    }

    smartPageIn(activePages, numActive, comingUp, numComingUp, available, numAvailable, q);
}

// proc: process to work upon (0-19)
// page: page to put in (0-19)
// returns
//   1 if pagein started or already started
//   0 if it can't start (e.g., swapping out)
// int pagein(int proc, int page);

// proc: process to work upon (0-19)
// page: page to swap out.
// returns:
//   1 if pageout started (not finished!)
//   0 if can't start (e.g., swapping in)
// int pageout(int proc, int page);

// first strategy: least-recently-used
// make a time variable, count ticks.
// make an array: what time each page was used for
// each process:
// int usedtime[PROCESSES][PAGES];
// At any time you need a new page, want to swap
// out the page that has least usedtime.

// second strategy: predictive
// track the PC for each process over time
// e.g., in a ring buffer.
// int pc[PROCESSES][TIMES];
// at any time pc[i][0]-pc[i][TIMES-1] are the last TIMES
// locations of the pc. If these are near a page border (up
// or down, and if there is an idle page, swap it in.

// Note: all data keeping must be in GLOBAL variables.