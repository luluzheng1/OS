#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define FALSE 0
#define TRUE 1

size_t GLOBALS = 0;

int rand()
{
    static int initialized = FALSE;
    static int (*original_rand)();
    if (!initialized)
    {
        original_rand = dlsym(RTLD_NEXT, "rand");
        fprintf(stderr, "Got rand symbol %x!\n", original_rand);
        initialized = TRUE;
    }
    int foo = original_rand();
    fprintf(stderr, "hoho! rand returns %d\n", foo);
    return 42; // most definitely not random!
}

int open(const char *path, int flags, mode_t mode)
{
    static int (*original_open)(const char *path, int flags, mode_t mode);
    original_open = dlsym(RTLD_NEXT, "open");
    pid_t tid = syscall(SYS_gettid);
    fprintf(stderr, "Child process with pid %d attempting to open a file, killing.\n", tid);
    kill(tid, 9);
    return -1;
}

FILE *fopen(const char *filename, const char *modes)
{
    FILE *(*original_fopen)(const char *filename, const char *modes);
    original_fopen = dlsym(RTLD_NEXT, "fopen");
    pid_t tid = syscall(SYS_gettid);
    fprintf(stderr, "Child process with pid %d attempting to open a file, killing.\n", tid);
    kill(tid, 9);
    return NULL;
}

pid_t fork(void)
{
    pid_t (*original_fork)(void);
    original_fork = dlsym(RTLD_NEXT, "fork");
    pid_t tid = syscall(SYS_gettid);
    fprintf(stderr, "Child process with pid %d attempting to fork, killing.\n", tid);
    kill(tid, 9);
    return -1;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    int (*original_pthread)(pthread_t * thread, const pthread_attr_t *attr, void *(routine)(void *), void *arg);
    original_pthread = dlsym(RTLD_NEXT, "pthread_create");
    pid_t tid = syscall(SYS_gettid);
    fprintf(stderr, "Child process with pid %d attempting to create a thread, killing.\n", tid);
    kill(tid, 9);
    return -1;
}

void *malloc(size_t size)
{
    void *(*original_malloc)(size_t size);
    void *ptr;
    if (size >= 4e6)
    {
        pid_t tid = syscall(SYS_gettid);
        fprintf(stderr, "Child with pid %d attempting to occupy more than 4 MB of heap memory, killing.\n", tid);
        kill(tid, 9);
        return NULL;
    }
    else if (size + GLOBALS >= 4e6)
    {
        pid_t tid = syscall(SYS_gettid);
        fprintf(stderr, "Child with pid %d attempting to occupy more than 4 MB of heap memory, killing.\n", tid);
        kill(tid, 9);
        return NULL;
    }
    GLOBALS += size;
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    ptr = original_malloc(size);
    return ptr;
}