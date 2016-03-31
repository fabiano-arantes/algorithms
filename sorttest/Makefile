CC=gcc
INC_DIR=-I ../util -I ../bubblesort/ -I ../heapsort/
#CFLAGS=-c -Wall -Wextra -Werror $(INC_DIR)
CFLAGS=-c -Wall -Wextra -Werror $(INC_DIR) -O0 -g -ggdb #-DDEBUG
OBJECTS=sorttest.o ../bubblesort/bubblesort.o ../heapsort/heapsort.o ../util/sortdata.o
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=sorttest

-include $(OBJS:.o=.d)

all: $(OBJECTS)

test: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) -MM $(CFLAGS) $*.c > $*.d
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(DEPS) $(EXECUTABLE)
