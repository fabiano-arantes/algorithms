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

    while ((p << 1) <= h->size)//mem_size(&h->mem))
    {
        size_t sizeof_pointer = sizeof(pointer_t) << 3;
        pointer_t p1;
        pointer_t p2;

        c = p << 1;

        if ((c + 1) <= h->size) //mem_size(&h->mem))
        {
            mem_get_bits_at(&h->mem, (c - 1) * sizeof_pointer, &p1, sizeof_pointer);
            mem_get_bits_at(&h->mem, c * sizeof_pointer,  &p2, sizeof_pointer);

            if (h->cmp_func(p1.pointer, p2.pointer) == 0)
            {
                ++c;
            }
        }

        mem_get_bits_at(&h->mem, (p - 1) * sizeof_pointer, &p1, sizeof_pointer);
        mem_get_bits_at(&h->mem, (c - 1) * sizeof_pointer, &p2, sizeof_pointer);

        if (h->cmp_func(p1.pointer, p2.pointer) == 0)
        {
            mem_exch(&h->mem, p - 1, c - 1);
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

    size_t c = h->size;//mem_size(&h->mem); //child index
    size_t p; //parent index
    size_t sizeof_pointer = sizeof(pointer_t) << 3;

    while (c > 1)
    {
        //get parent index
        p = c >> 1;

        pointer_t v1, v2;

        mem_get_bits_at(&h->mem, (p - 1) * sizeof_pointer, &v1, sizeof_pointer);
        mem_get_bits_at(&h->mem, (c - 1) * sizeof_pointer, &v2, sizeof_pointer);

        //check if values positions are valid
        if (h->cmp_func(v1.pointer, v2.pointer))
        {
            //valid positions
            break;
        }

        //exchange values in p and c
        mem_exch(&h->mem, p - 1, c -1);

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

    pointer_t p = {value};

    //put new value at last position of heap tree
    MEM_APPEND_VALUE(&h->mem, &p);

    ++h->size;

    //apply swim
    heap_swim(h);

    return h->size;
}

void* heap_remove_top(sort_data* const h)
{
    MSG("Removing top from heap");

    if (h->size == 0)
    {
        MSG("Heap is empty");
        return NULL;
    }

    void *ptop = NULL;

    mem_seek(&h->mem, SEEK_READ_RESET);

    //get top value from memory
    if (MEM_GET_NEXT_VALUE(&h->mem, &ptop))
    {
        MSG_ARG("Top object popped: [%p]", ptop);

        --h->size;

        mem_exch(&h->mem, 0, h->size);

        if((mem_shrink(&h->mem, sizeof(void *) << 3)) && heap_sink(h))
        {
            return ptop;
        }

        MSG("mem_shrink or heap_sink failed");
    }

    MSG("heap_remove_top failed");

    return NULL;
}
