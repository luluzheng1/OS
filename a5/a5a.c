// (a5a.c) measure the speed with which one can read from the
// network disk containing your home directory and class directory
// (it's the same for everyone).
#define _POSIX_C_SOURCE 199309

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define BUF_SIZE 8192
#define PATH "./tmpFiles/"
#define ITERATIONS 1000

static inline double get_seconds(time_t start_sec, time_t end_sec, long start_nsec, long end_nsec)
{
    long seconds = end_sec - start_sec;
    long ns = end_nsec - start_nsec;

    if (start_nsec > end_nsec)
    {
        --seconds;
        ns += 1000000000;
    }

    return (double)seconds + (double)ns / 1e9;
}

static inline int getRandom(int i)
{
    return i + rand() % 50;
}

static inline double getLoopTime()
{
    int j;
    (void)j;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < ITERATIONS; i++)
    {
        j = getRandom(i);
    }
    clock_gettime(CLOCK_REALTIME, &end);

    return get_seconds(start.tv_sec, end.tv_sec, start.tv_nsec, end.tv_nsec);
}

int main()
{
    double seconds = 0.0;
    char *buffer = (char *)malloc(BUF_SIZE);
    buffer[BUF_SIZE - 1] = '\0';
    struct timespec start, end;
    mkdir(PATH, 0777);
    // Create files of size 8192 bytes
    char filePath[50];
    int fd;
    for (int i = 0; i < ITERATIONS; i++)
    {
        sprintf(filePath, "%s%d.dat", PATH, i);
        fd = open(filePath, O_WRONLY | O_CREAT, 0666);
        write(fd, buffer, BUF_SIZE);
        close(fd);
    }

    // Open the files
    int fds[ITERATIONS];
    for (int i = 0; i < ITERATIONS; i++)
    {
        sprintf(filePath, "%s%d.dat", PATH, i);
        fds[i] = open(filePath, O_RDONLY);
    }

    clock_gettime(CLOCK_REALTIME, &start);
    // Measure time for reads
    for (int i = 0; i < ITERATIONS; i++)
    {
        read(fds[i], buffer, BUF_SIZE);
        getRandom(i);
    }
    clock_gettime(CLOCK_REALTIME, &end);

    // Delete files
    for (int i = 0; i < ITERATIONS; i++)
    {
        sprintf(filePath, "%s%d.dat", PATH, i);
        unlink(filePath);
    }
    rmdir(PATH);
    seconds = get_seconds(start.tv_sec, end.tv_sec, start.tv_nsec, end.tv_nsec);
    seconds -= getLoopTime();
    seconds /= (double)ITERATIONS;
    printf("%e seconds to read 8192 bytes from network disk\n", seconds);
    return 0;
}
