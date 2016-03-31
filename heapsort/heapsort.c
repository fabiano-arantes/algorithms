#include "heapsort.h"

#include <debug.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int heap_sink(sort_data *h)
{
    MSG("Sinking heap");

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

        if (((c + 1) <= h->size) && (h->cmp_func(get_obj_address(h, c - 1), get_obj_address(h, c)) == 0))
        {
            ++c;
        }

        if(h->cmp_func(get_obj_address(h, p - 1), get_obj_address(h, c - 1)) == 0)
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
    MSG("Swimming heap");

    size_t c = h->size; //child index
    size_t p; //parent index

    while (c > 1)
    {
        //get parent index
        p = c >> 1;

        //check if values positions are valid
        if (h->cmp_func(get_obj_address(h, p - 1), get_obj_address(h, c - 1)))
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
    MSG("Putting new value in heap");

    if (h == NULL)
    {
        MSG("h NULL pointer");
        return 0;
    }

    if (h->cmp_func == NULL)
    {
        MSG("Comparison function not set");
        return 0;
    }

    //put new value at last position of heap tree
    mem_put_value(h, value);

    //apply swim
    heap_swim(h);

    return h->size;
}

void* heap_remove_top(sort_data* const h)
{
    void* top;

    MSG("Removing top from heap");

    if (h->size == 0)
    {
        MSG("Heap is empty");
        return NULL;
    }

    //get top value from memory
    if ((top = mem_get_value(h, 0)) != NULL)
    {
        MSG_ARG("Top object popped: [%p]", top);

        exch(h, 0, h->size - 1);

        if((mem_shrink(h)) && heap_sink(h))
        {
            return top;
        }

        MSG("mem_shrink or heap_sink failed");
    }

    MSG("heap_remove_top failed");

    return NULL;
}
