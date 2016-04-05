CC=gcc
INC_DIR=-I ../../util
CFLAGS=-c -Wall -Wextra -Werror -g -ggdb -DDEBUG $(INC_DIR)
OBJECTS=trie3w.o testtrie3w.o
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=testtrie3w

-include $(DEPS)

all: $(OBJECTS)

test: $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $*.c > $*.d

clean:
	rm -r $(OBJECTS) $(DEPS) $(EXECUTABLE) $(EXECUTABLE).dSYM
