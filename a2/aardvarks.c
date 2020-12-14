#include "/comp/111/assignments/a2/anthills.h"
#include "semaphore.h"
#define TRUE 1
#define FALSE 0
#define NO_MORE_ANTS(h) (NUM_ANTS[h] <= 0 ? 1 : 0)
int initialized = FALSE; // semaphores and mutexes are not initialized

void my_slurp(char aname);
int next_hill(int aardvark);
void *slurp_time(void *arg);
int decrement_ant_count(int aardvark, int hill);
// define your mutexes and semaphores here
// they must be global variables.
sem_t sems[3];
pthread_mutex_t mutex[11];

// indices for each hill
int HILLS[3] = {0, 1, 2};
int curr_hill = 0;
int NUM_ANTS[3] = {100, 100, 100};

// first thread initializes mutexes
void *aardvark(void *input)
{
    char aname = *(char *)input;
    // first caller needs to initialize the mutexes!
    pthread_mutex_lock(&init_lock);
    if (!initialized++)
    { // this succeeds only for one thread
        // initialize your mutexes and semaphores here
        // 3 semaphores for 3 anthills
        for (int i = 0; i < 3; i++)
        {
            sem_init(&sems[i], 0, 3);
        }
        // 11 mutex for 11 aardvarks
        for (int i = 0; i < 11; i++)
        {
            pthread_mutex_init(&mutex[i], NULL);
        }
    }
    pthread_mutex_unlock(&init_lock);

    // put some aardvarks to sleep first
    if (aname % 2)
    {
        sleep(1);
    }
    // now be an aardvark
    while (chow_time())
    {
        my_slurp(aname);
    }

    return NULL;
}

void my_slurp(char aname)
{
    int hill, count;
    int aardvark = aname - 'A';
    pthread_t swallow;

    hill = next_hill(aardvark);
    if (NO_MORE_ANTS(hill))
        return;

    // check if that hill is available
    while (sem_trywait(&sems[hill]) == -1)
    {
        // go to a new hill if not
        hill = next_hill(aardvark);
        if (NO_MORE_ANTS(hill))
            return;
    }

    // unlock hill after slurping
    pthread_create(&swallow, NULL, &slurp_time, &hill);

    if (decrement_ant_count(aardvark, hill) <= 0)
        return;

    // slurp failed, undo decrement
    if (slurp(aname, hill) == 0)
        NUM_ANTS[hill]++;

    pthread_join(swallow, NULL);
    return;
}

void *slurp_time(void *arg)
{
    int hill = *(int *)arg;

    sleep(1);

    sem_post(&sems[hill]);
}

int next_hill(int aardvark)
{
    pthread_mutex_lock(&mutex[aardvark]);
    int hill = HILLS[curr_hill % 3];
    pthread_mutex_unlock(&mutex[aardvark]);
    curr_hill++;

    return hill;
}

int decrement_ant_count(int aardvark, int hill)
{
    pthread_mutex_lock(&mutex[aardvark]);
    int count = NUM_ANTS[hill]--;
    pthread_mutex_unlock(&mutex[aardvark]);

    return count;
}
// currently averaging 56 seconds, what could I be doing better?