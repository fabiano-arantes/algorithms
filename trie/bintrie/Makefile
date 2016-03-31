CC=gcc
INC_DIR=-I ../../util -I ../../heapsort
CFLAGS=-c -Wall -Wextra -Werror -O0 -g -ggdb $(INC_DIR) #-DDEBUG 
#CFLAGS=-c -Wall -Wextra -Werror $(INC_DIR)
OBJECTS=bintrie.o testbintrie.o ../../heapsort/heapsort.o ../../util/sortdata.o
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=testbintrie

-include $(DEPS)

all: $(OBJECTS)

test: $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $*.c > $*.d

clean:
	rm $(OBJECTS) $(DEPS) $(EXECUTABLE)
