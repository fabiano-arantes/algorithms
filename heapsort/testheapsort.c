#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "heapsort.h"

static sort_data heap;
static int values[] = {5, 2, 4, 3, 1, 0, 7};
static int sorted_min_values[] = {0, 1, 2, 3, 4, 5, 7};
static int sorted_max_values[] = {7, 5, 4, 3, 2, 1, 0};

void test(int base_array[], size_t size)
{
    //get array size
    size_t i;
    int top;

    heap.type_size = sizeof(int);

    //fill array
    for (i = 0; i < size; ++i)
    {
        heap_put(&heap, &values[i]);
    }

    //pop values
    for (i = 0; i < size; ++i)
    {
        if((heap_remove_top(&heap, &top) == 0) || (top != base_array[i]))
        {
            printf("(%zu) value: %d, expected: %d\n", i + 1, top, base_array[i]);
        }
    }
}

int main()
{
    //get array size
    size_t size = sizeof(values) / sizeof(*values);

    //set all to 0
    memset(&heap, 0, sizeof(sort_data));

    //test ascending
    heap.cmp_func = asc_cmp_int;
    test(sorted_min_values, size);

    //test descending
    heap.cmp_func = desc_cmp_int;
    test(sorted_max_values, size);

    free(heap.mem);

    return 0;
}
