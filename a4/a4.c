#include <stdio.h>
#include <stdlib.h>
#include "/comp/111/assignments/a4/t4.h"

size_t has_free_pages(Pentry q[MAXPROCESSES], int proc)
{
   int page;
   size_t free = 0;

   for (page = 0; page < MAXPROCPAGES; page++)
   {
      if (q[proc].pages[page])
         free++;
   }

   return free > 0 ? 1 : 0;
}

void pageit_lru(Pentry q[MAXPROCESSES], int ts[MAXPROCESSES][MAXPROCPAGES], int proc, int tick, int *oldpage)
{
   int page;
   int t;

   *oldpage = -1;
   t = tick + 1; // Adding 1 to tick to make sure lru_page always return a page

   for (page = 0; page < MAXPROCPAGES; page++)
   {
      // Not available, move on
      if (!q[proc].pages[page])
         continue;

      if (ts[proc][page] < t)
      {
         t = ts[proc][page];
         *oldpage = page;

         // This is the best case
         if (t <= 1)
            break;
      }
   }

   if (*oldpage < 0)
   {
      printf("could not find page with process %d with age %d\n", proc, tick);
      exit(EXIT_FAILURE);
   }
}

void pageit(Pentry q[MAXPROCESSES])
{
   // these are globals that only the subroutine can see
   static int tick = 0; // artificial time
   static int timestamps[MAXPROCESSES][MAXPROCPAGES];
   // these are regular dynamic variables on stack
   int proc, pc, page, oldpage;

   // select first active process
   for (proc = 0; proc < MAXPROCESSES; proc++)
   {
      // if active, then work on it ONLY
      if (q[proc].active)
      {
         pc = q[proc].pc;               // program counter for process
         page = pc / PAGESIZE;          // page the program counter needs
         timestamps[proc][page] = tick; // last access

         if (!q[proc].pages[page])
         { // if page is not there:
            if (!pagein(proc, page))
            {
               // If there are at least one page to evict
               if (has_free_pages(q, proc))
               {
                  pageit_lru(q, timestamps, proc, tick, &oldpage);
                  if (!pageout(proc, oldpage))
                     exit(EXIT_FAILURE);
               }
            }
         }
      }
      continue;
   }
   tick++; // advance time for next iteration
}
