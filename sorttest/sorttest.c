#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include <bubblesort.h>
#include <heapsort.h>

typedef int fp_put(sort_data * const, const void* const);
typedef void* fp_remove_top(sort_data * const);

static sort_data sorted_values;

void sort_test(FILE* const f, fp_put* const sort_put, fp_remove_top* const sort_remove_top)
{
    unsigned int count_load = 0;
    unsigned int count_remove = 0;
    int value;
    int *v;

    clock_t t_load = clock();

    while (fscanf(f, "%d", &value) && !feof(f))
    {
        MSG_ARG("putting %d", value);

        ++count_load;
        sort_put(&sorted_values, &value);
    }

    t_load = clock() - t_load;

    clock_t t_remove = clock();

    while ((v = sort_remove_top(&sorted_values)) != NULL)
    {
        ++count_remove;

        MSG_ARG("[%d] = %d", count_remove, *v);
    }

    t_remove = clock() - t_remove;

    printf("time to load = %lu - count = %d\n", t_load, count_load);
    printf("time to empty = %lu - count = %d\n", t_remove, count_remove);

}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <file with list of numbers>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        MSG_ARG("Fail to open file %s", argv[1]);

        return 1;
    }

    sorted_values.cmp_func = asc_cmp_int;

    sort_test(f, &heap_put, &heap_remove_top);

    rewind(f);

    sort_test(f, &bubble_put, &bubble_remove_top);

    fclose(f);

    return 0;
}
