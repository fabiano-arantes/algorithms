#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include <bubblesort.h>
#include <heapsort.h>

typedef int fp_put(sort_data * const, const void* const);
typedef int fp_remove_max(sort_data * const, void* const);

int main(int argc, char **argv)
{
    sort_data sorted_values;
    int value;
    unsigned int count_load = 0;
    unsigned int count_remove = 0;
    fp_put *sort_put;
    fp_remove_max *sort_remove_max;

    if (argc < 3)
    {
        printf(
                "Usage: %s <file with list of numbers> <sort type: heap(h), bubble(b)>\n",
                argv[0]);
        return 1;
    }

    if (strstr(argv[2], "h"))
    {
        sort_put = &heap_put;
        sort_remove_max = &heap_remove_top;
    }
    else if (strstr(argv[2], "b"))
    {
        sort_put = &bubble_put;
        sort_remove_max = &bubble_remove_top;
    }

    sorted_values.type_size = sizeof(int);

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        MSG_ARG("Fail to open file %s", argv[1]);

        return 1;
    }

    clock_t t_load = clock();

    while (fscanf(f, "%d", &value) && !feof(f))
    {
        MSG_ARG("putting %d", value);

        ++count_load;
        sort_put(&sorted_values, &value);
    }

    t_load = clock() - t_load;

    fclose(f);

    clock_t t_remove = clock();

    while (sort_remove_max(&sorted_values, &value))
    {
        ++count_remove;

        MSG_ARG("[%d] = %d", count_remove, value);
    }

    t_remove = clock() - t_remove;

    printf("time to load = %lu - count = %d\n", t_load, count_load);
    printf("time to empty = %lu - count = %d\n", t_remove, count_remove);

    return 0;
}
