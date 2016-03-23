#include "heapsort.h"

#include <debug.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int heap_sink(sort_data *h)
{
    if (h == NULL)
    {
        MSG("h NULL pointer");

        return 0;
    }

    size_t p = 1;
    size_t c;

    while ((p << 1) <= h->size)
    {
        c = p << 1;

        if (((c + 1) <= h->size) && (h->cmp_func(h, c - 1, c) == 0))
        {
            ++c;
        }

        if(h->cmp_func(h, p - 1, c - 1) == 0)
        {
            exch(h, p - 1, c - 1);
        }
        else
        {
            break;
        }

        p = c;
    }

    return p;
}

static void heap_swim(sort_data* const h)
{
    size_t c = h->size; //child index
    size_t p; //parent index

    while (c > 1)
    {
        //get parent index
        p = c >> 1;

        //check if values positions are valid
        if (h->cmp_func(h, p - 1, c - 1))
        {
            //valid positions
            break;
        }

        //exchange values in p and c
        exch(h, p - 1, c -1);

        //parent now is the new child
        c = p;
    }
}

int heap_put(sort_data* const h, const void * const value)
{
    if (h == NULL)
    {
        MSG("h NULL pointer");
        return 0;
    }

    //put new value at last position of heap tree
    mem_put_value(h, value);

    //apply swim
    heap_swim(h);

    return h->size;
}

int heap_remove_top(sort_data* const h, void* const top)
{
    //get top value from memory
    if (mem_get_value(h, 0, top))
    {
        exch(h, 0, h->size - 1);

        if(mem_shrink(h))
        {
            //apply sink
            return heap_sink(h);
        }
    }

    return 0;
}
