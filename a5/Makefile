CFLAGS=-g -std=c99 -Wall -Wextra -pedantic
OUT_FILES=a5a.out a5b.out a5c.out a5d.out a5e.out a5f.out a5g.out

all: $(OUT_FILES)

.c.out:
	gcc $(CFLAGS) -o $@ $< -lrt

clean:
	rm -f *.o $(OUT_FILES)
