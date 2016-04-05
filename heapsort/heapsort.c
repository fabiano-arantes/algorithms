#include "heapsort.h"

#include <debug.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned bit: 3;
 //   void *pointer;
}pointer_t;

static int heap_sink(sort_data *h)
{
    MSG("Sinking heap");

    if (h == NULL)
    {
        MSG("h NULL pointer");

        return 0;
    }

    pointer_t ab;

    ab.bit = 7;

    size_t p = 1;
    size_t c;
    void * v1;
    void * v2;

    while ((p << 1) <= h->size)//mem_size(&h->mem))
    {
        c = p << 1;


        if ((c + 1) <= h->size) //mem_size(&h->mem))
        {
            v1 = *(void **)mem_get_obj_address(&h->mem, c - 1, sizeof(void *));
            v2 = *(void **)mem_get_obj_address(&h->mem, c, sizeof(void *));

            if (h->cmp_func(v1, v2) == 0)
            {
                ++c;
            }
        }

        v1 = *(void **)mem_get_obj_address(&h->mem, p - 1, sizeof(void *));
        v2 = *(void **)mem_get_obj_address(&h->mem, c - 1, sizeof(void *));

        if(h->cmp_func(v1, v2) == 0)
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
    void * v1;
    void * v2;

    while (c > 1)
    {
        //get parent index
        p = c >> 1;

        v1 = *(void **)mem_get_obj_address(&h->mem, p - 1, sizeof(void *));
        v2 = *(void **)mem_get_obj_address(&h->mem, c - 1, sizeof(void *));

        //check if values positions are valid
        if (h->cmp_func(v1, v2))
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

    //put new value at last position of heap tree
    MEM_APPEND_VALUE(&h->mem, &value);

    ++h->size;

    //apply swim
    heap_swim(h);

    return h->size;//mem_size(&h->mem);
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
    if (MEM_GET_NEXT_VALUE(&h->mem, &ptop)) //second must address of address because heap deals with
    {                                      //memory addresses in mem
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
