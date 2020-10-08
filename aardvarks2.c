#include "anthills.h"
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#define SIG SIGRTMIN
#define TRUE 1
#define FALSE 0

int initialized = FALSE; // semaphores and mutexes are not initialized

/*ant count*/
int COUNTS[3] = {100, 100, 100};

/*hill array preserves ordering, HILLPTR global index iterator*/
int HILLS[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
int HILLPTR = 0;

/*global semaphores, mutexes, and timespec structs*/
sem_t sems[3];
pthread_mutex_t threads[11];
struct timespec offset = {1, 1000000};
struct timespec rem = {0, 0};
struct timespec set = {1, 1000000};

int lock_hill(int index);
void my_slurp(char aname); //, timer_t timerid, struct itimerspec *its);
int update_ant_count(int hill, int index);
static void *time_it(void *arg);

/*main arradvark thread function*/
void *aardvark(void *input)
{
    char aname = *(char *)input;
    int i, index;

    pthread_mutex_lock(&init_lock);
    if (!initialized++)
    {
        for (i = 0; i < 3; i++)
        {
            sem_init(&sems[i], 0, 3);
        }
        for (i = 0; i < 11; i++)
        {
            pthread_mutex_init(&threads[i], NULL);
        }
    }
    pthread_mutex_unlock(&init_lock);
    if (aname % 2 == 0)
    { //put half the threads to sleep for ~1.1 seconds
        nanosleep(&offset, NULL);
    }
    while (chow_time())
    {
        my_slurp(aname);
    }
    return NULL;
}

/*
 * Lock the hill, an diterate through an array to return 
 * hill values in an ordered fashion
 */
int lock_hill(int index)
{
    pthread_mutex_lock(&threads[index]);
    int hill = HILLS[HILLPTR % 9];
    HILLPTR++;
    pthread_mutex_unlock(&threads[index]);
    return hill;
}

/* 
 * Update ant count at the hill prior to
 * slurping the hill if the anthill is empty, return true
 */
int update_ant_count(int hill, int index)
{
    pthread_mutex_lock(&threads[index]);
    COUNTS[hill] -= 1;
    pthread_mutex_unlock(&threads[index]);
    if (COUNTS[hill] < 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* 
 * wrapper function for slurp to check semaphores for a given hill
 * if hill is available, spawn a thread to time self to
 * decrement the semaphore once swallowing has commenced
 * otherwise get a new hill and commence checking
 */
void my_slurp(char aname)
{
    int check_sem, index = aname - 'A', hill;
    pthread_t timer_thread;
newhill:
    hill = lock_hill(index);
    if (COUNTS[hill] <= 0)
    {
        return;
    }
    if ((check_sem = sem_trywait(&sems[hill])) == -1)
    {
        goto newhill;
    }
    else
    {
        pthread_create(&timer_thread, NULL, &time_it, &hill);
        goto slurp;
    }
slurp:
    if (update_ant_count(hill, index))
    {
        return;
    }
    if (slurp(aname, hill) == FALSE)
    {
        COUNTS[hill] += 1;
    }
    pthread_join(timer_thread, NULL);
    return;
}

/*
 * Spawn a thread to time the parent thread and decrement the hill sem
 */
static void *time_it(void *arg)
{
    int hill = *(int *)arg;
    while (nanosleep(&offset, &rem) != 0)
    {
        offset = rem;
    }
    offset = set;
    sem_post(&sems[hill]);
}