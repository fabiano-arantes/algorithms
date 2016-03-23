CC=gcc
INCLUDES=-I../util
#CFLAGS=-c -Wall -Wextra -pedantic -g -ggdb -DDEBUG
CFLAGS=-c -Wall -Wextra -ggdb
LDFLAGS=
SOURCES=heapsort.c testheapsort.c ../util/sortdata.c
EXECUTABLE=testheapsort
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)  

test: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE) 
