CC = gcc
CFLAGS = -g -std=c99 -pedantic
LDLIBS = -lrt -lpthread

all: a2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

a2: aardvarks.o anthills.o
	$(CC) $^ -o $@ $(LDLIBS)

clean:
	rm -f a2 *.o
