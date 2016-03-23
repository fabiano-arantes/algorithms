CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb3

all: trie.o

test: testtrie 

testtrie: trie.o testtrie.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -r *.o *.dSYM
