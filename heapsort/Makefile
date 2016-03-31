CC=gcc
INC_DIR=-I ../util
#CFLAGS=-c -Wall -Wextra -Werror $(INC_DIR)
CFLAGS=-c -Wall -Wextra -Werror $(INC_DIR) -O0 -g -ggdb #-DDEBUG
OBJECTS=heapsort.o testheapsort.o ../util/sortdata.o
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=testheapsort

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
