CC=gcc
INC_DIR=-I ../util
CFLAGS=-c -Wall -Wextra -Werror -g -ggdb -DDEBUG $(INC_DIR)
OBJECTS=heapsort.o testheapsort.o ../util/sortdata.o
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=testheapsort

-include $(OBJS:.o=.d)

all: $(OBJECTS)

test: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $*.c > $*.d

clean:
	rm $(OBJECTS) $(DEPS) $(EXECUTABLE)
