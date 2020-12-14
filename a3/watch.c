/*
Compile method: Please compile using the Makefile.
I believe all cases are handled by watch.c except for the extra credit.

Case 1: limit size of stack. 
Handled by setrlimit() where stack memory is limited to 4MB.

Case 2: limit size of heap. 
Handled by limiting heap size to 4MB via redefining malloc() in shadow.c.

Case 3: prohibit forking. 
Handled by redefining fork() in shadow.c.

Case 4: prohibit thread creation. 
Handled by redefining pthread_create() in shadow.c.

Case 5: prohibit opening files. 
Handled by redefining open() and fopen() in shadow.c.

(extra credit) Case 6: Limit size of global variables.
Not Handled.
*/
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>

#define _XOPEN_SOURCE 700
#define SIG 11
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

// Forward declarations
void reap_handler(int sig);
void capture_prints_by_child(FILE *pFile);
void total_runtime(char **argv);

pid_t CHILD;      // child's process ID
int FIN = 0;      // set to 1 when child terminates
int NUMLINES = 0; // number of lines printed to stdout by child
struct rusage START = {0};
struct rusage END = {0};
time_t DEATH;
void reap_handler(int sig)
{
    // Catch malicious child behavior
    if (sig == SIGINT || sig == SIGSEGV || sig == SIGUSR1)
    {
        fprintf(stderr, "Received malicious signal %d from child, killing now.\n", sig);
        kill(CHILD, 9);
    }
    // Wait for the child to terminate
    int status;
    pid_t child = waitpid(0, &status, WNOHANG);
    if (child != CHILD && child > 0)
        return;
    else if (child < 0)
    {
        // Errors
        if (errno == ECHILD)
            fprintf(stderr, "Error - waitpid returned %s, child does not exist\n", child);
        else if (errno == EINTR)
            fprintf(stderr, "WNOHANG not set and an unblocked signal was caught\n");
        else
            fprintf(stderr, "the options argument was invalid\n");

        exit(1);
    }
    else if (child > 0)
    {
        getrusage(RUSAGE_SELF, &END);
        time(&DEATH);
        // Child terminated normally
        if (WIFEXITED(status))
        {
            fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(status));
        }
        // Child terminated by a signal
        else if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == 11)
            {
                fprintf(stderr, "Child with pid %d attempted to occupy more than 4MB of stack memory.\n", child);
            }
            fprintf(stderr, "Child terminated by signal %d\n", WTERMSIG(status));
        }
    }
    FIN = 1;
    return;
}

void capture_prints_by_child(FILE *pFile)
{
    char buffer[512];
    while (FIN == 0)
    {
        if (feof(pFile) != 0 || ferror(pFile) != 0)
            return;
        if (fgets(buffer, sizeof(buffer), pFile) == NULL)
            return;
        NUMLINES += 1;
        // Write to stderr
        fputs(buffer, stderr);
    }
}

void total_runtime(char **argv)
{
    struct rusage *start = &START;
    struct rusage *end = &END;

    double user_total = (((double)end->ru_utime.tv_sec - (double)start->ru_utime.tv_sec) + ((double)end->ru_utime.tv_usec - (double)start->ru_utime.tv_usec)) / 1e6;

    double system_total = (((double)end->ru_stime.tv_sec - (double)start->ru_stime.tv_sec) + ((double)end->ru_stime.tv_usec - (double)start->ru_stime.tv_usec)) / 1e6;

    double total = user_total + system_total;
    if (total < 0)
        total *= -1;
    fprintf(stderr, "Total runtime for %s: %e\n", argv[1], total);
    return;
}

void main(int argc, char **argv, char **envp)
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
        exit(0);
    }

    struct sigaction act;
    // Reap child
    act.sa_handler = reap_handler;

    sigemptyset(&act.sa_mask);
    // Set stack limit to 4 MB
    struct rlimit stack_lim = {.rlim_cur = 4e6, .rlim_max = 4e6};

    FILE *pFile;
    int pipefd[2]; // Establish pipe between parent and child
    if (pipe(pipefd) < 0)
    {
        fprintf(stderr, "could not create pipe: %d", strerror(errno));
        exit(0);
    }

    // On error, return unsuccessfully
    if (sigaction(SIGSEGV, &act, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed with errno %d", strerror(errno));
        exit(0);
    }
    if (sigaction(SIGCHLD, &act, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed with errno %d", strerror(errno));
        exit(0);
    }
    if (sigaction(SIGINT, &act, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed with errno %d", strerror(errno));
        exit(0);
    }

    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed with errno %d", strerror(errno));
        exit(0);
    }
    if (sigaction(SIG, &act, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed with errno %d", strerror(errno));
        exit(0);
    }
    if (CHILD = fork())
    {
        getrusage(RUSAGE_SELF, &START);
    }
    else
    {
        // run child with shadow library.
        // set stack limit to 4MB
        if (setrlimit(RLIMIT_STACK, &stack_lim) == -1)
        {
            fprintf(stderr, "failed to set stack limit to 4 MB: %s\n", strerror(errno));
            exit(0);
        }
        setenv("LD_PRELOAD", "./shadow.so", 1);
        unsetenv("LD_LIBRARY_PATH");
        close(1);
        dup(pipefd[1]);
        close(pipefd[1]);
        close(pipefd[0]);
        if (execl(argv[1], argv[1], NULL) == -1)
        {
            fprintf(stderr, "Executing child process failed with errno %s", strerror(errno));
            exit(0);
        }
    }

    pFile = fdopen(pipefd[0], "r");
    close(pipefd[1]);
    capture_prints_by_child(pFile);
    total_runtime(argv);
    fprintf(stderr, "Mumber of lines printed to stdout: %d\n", NUMLINES);
    fprintf(stderr, "Wallclock time of death: %s", ctime(&DEATH));
    return;
}
