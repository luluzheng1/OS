CFLAGS= -std=c99 -g
all: a.out see.R test2
a.out: t4.o a4.o predictive.o
	gcc $(CFLAGS) t4.o a4.o predictive.o

test: test.c t4.c
	gcc $(CFLAGS) -o test test.c t4.c 
test2: test2.c t4.c
	gcc $(CFLAGS) -o test2 test2.c t4.c
predictive: predictive.c t4.c
	gcc $(CFLAGS) -o predictive predictive.c t4.c
t4.o: t4.c /comp/111/assignments/a4/t4.h 
	gcc $(CFLAGS) -c t4.c 
a4.o: a4.c /comp/111/assignments/a4/t4.h 
	gcc $(CFLAGS) -c a4.c 
test.o: test.c /comp/111/assignments/a4/t4.h 
	gcc $(CFLAGS) -c test.c
t4.c: /comp/111/assignments/a4/t4.c
	ln -s /comp/111/assignments/a4/t4.c .
see.R: /comp/111/assignments/a4/see.R
	ln -s /comp/111/assignments/a4/see.R .
t4.h: /comp/111/assignments/a4/t4.h 
	ln -s /comp/111/assignments/a4/t4.h .

clean:
	rm -f a.out