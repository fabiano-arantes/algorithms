CC=gcc
INCLUDES=-I../util
CFLAGS=-c -Wall -Wextra -g -ggdb -DDEBUG
LDFLAGS=
SOURCES=bubblesort.c testbubblesort.c ../util/sortdata.c
EXECUTABLE=testbubblesort
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)  

test: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $^ -o $@

clean:
	rm *.o $(EXECUTABLE) 
