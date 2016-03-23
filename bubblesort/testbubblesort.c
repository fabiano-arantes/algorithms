#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bubblesort.h"
#include "debug.h"


static sort_data bubble;
static int values[] = {2, 5, 4, 3, 1, 0, 7};
static int sorted_min_values[] = {0, 1, 2, 3, 4, 5, 7};
static int sorted_max_values[] = {7, 5, 4, 3, 2, 1, 0};

void test(int base_array[], size_t size)
{
    //get array size
    size_t i;
    int v;

    //fill array
    for (i = 0; i < size; ++i)
    {
        if (bubble_put(&bubble, &values[i]) == 0)
        {
            MSG_ARG("Error on insert value = %d", values[i]);
            return;
        }
    }

    //pop values
    for (i = 0; i < size; ++i)
    {
        bubble_remove_top(&bubble, &v);
        if (v != base_array[i])
        {
            printf("(%zu) value: %d, expected: %d\n", i, v, base_array[i]);
        }
    }
}

int main()
{
    //get array size
    size_t size = sizeof(values) / sizeof(*values);

    //set all to 0
    memset(&bubble, 0, sizeof(sort_data));

    bubble.type_size = sizeof(*values);

    //test ascending
    bubble.cmp_func = asc_cmp_int;
    test(sorted_min_values, size);

    //test descending
    bubble.cmp_func = desc_cmp_int;
    test(sorted_max_values, size);

    free(bubble.mem);

    return 0;
}
