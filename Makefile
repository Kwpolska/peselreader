CFLAGS=-Wall -pedantic -g -DDEBUG -std=c99 -D_POSIX_C_SOURCE=200809L
# <3 GCC
POSTFLAGS=-lm
PROG=pesel-reader

all:
	$(CC) $(CFLAGS) $(PROG).c $(POSTFLAGS) -o $(PROG)

test:
	$(CC) $(CFLAGS) test-pesel-reader.c $(POSTFLAGS) -o test-pesel-reader
	./test-pesel-reader
