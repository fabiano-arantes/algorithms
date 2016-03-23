CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb3

all: huffman.o
	$(CC) $< -o huffman

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -r *.o huffman *.dSYM
