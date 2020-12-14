CFLAGS=-g -std=c99


all: watch shadow_test shadow.so 1.out 1b.out 2.out 2b.out 3.out 3b.out 4.out 4b.out 5.out 5b.out 6.out

watch: watch.c
	gcc $(CFLAGS) -o watch watch.c 

shadow_test: shadow_test.c
	gcc $(CFLAGS) -o shadow_test shadow_test.c 

shadow.so: shadow.c
	gcc -shared -fPIC $(CFLAGS) shadow.c -o shadow.so -ldl

1.out: 1.c
	gcc $(CFLAGS) -o 1.out 1.c
2.out: 2.c
	gcc $(CFLAGS) -o 2.out 2.c
3.out: 3.c 
	gcc $(CFLAGS) -o 3.out 3.c
4.out: 4.c
	gcc $(CFLAGS) -o 4.out 4.c -lpthread
5.out: 5.c
	gcc $(CFLAGS) -o 5.out 5.c
6.out: 6.c
	gcc $(CFLAGS) -o 6.out 6.c
1b.out: 1b.c
	gcc $(CFLAGS) -o 1b.out 1b.c
2b.out: 2b.c
	gcc $(CFLAGS) -o 2b.out 2b.c
3b.out: 3b.c 
	gcc $(CFLAGS) -o 3b.out 3b.c
4b.out: 4b.c
	gcc $(CFLAGS) -o 4b.out 4b.c -lpthread
5b.out: 5b.c
	gcc $(CFLAGS) -o 5b.out 5b.c

clean: 
	-rm watch shadow_test shadow.so 1.out 1b.out 2.out 2b.out 3.out 3b.out 4.out 4b.out 5.out 5b.out 6.out

