#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
	#define __DEBUG_HEADER__ "DEBUG : %s:%s() (%d) : "
	#define DBG(x, ...) printf(__DEBUG_HEADER__ x "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
	#define DBG(x, ...)
#endif

typedef struct str_node_int{
	int d;
	struct str_node_int *next;
} node_int;

node_int *new_node_int(void);

void fill_linked_list_int(node_int **phead);

void print_linked_list_int(node_int *head);

void erase_linked_list_int(node_int* head);
