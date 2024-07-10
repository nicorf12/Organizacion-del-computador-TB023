CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o lib.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c cache.h
	$(CC) $(CFLAGS) -c $< -o $@

lib.o: lib.c cache.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f cachesim
	

