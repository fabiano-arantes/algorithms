#include "sortdata.h"

#include "debug.h"

#include <stdlib.h>

void* mem_shrink(sort_data* const n)
{
    if (n->size == 0)
    {
        MSG("heap is empty");
        return NULL;
    }

    //decrease one position
    --n->size;

    if ((n->max_size > 1) && (n->size <= (n->max_size >> 2)))
    {
        n->max_size >>= 1;
        n->mem = realloc(n->mem, n->max_size * sizeof(void*));

        if (n->mem == NULL)
        {
            MSG("Error on reallocate memory");

            n->max_size = 0;
            n->size = 0;

            return NULL;
        }
    }

    return n->mem + (n->size * sizeof(void*));
}

void* mem_get_value(sort_data* const h, size_t index)
{
    if (h == NULL)
    {
        MSG("node is NULL");
        return 0;
    }

    if (h->mem == NULL)
    {
        MSG("mem is NULL");
        return 0;
    }

    return get_obj_address(h, index);
}

static void* mem_get_free_slot(sort_data* const h)
{
    if (h == NULL)
    {
        MSG("h is null");
        return NULL;
    }

    //check if mem was allocated
    if (h->mem == NULL)
    {
        h->mem = malloc(sizeof(void*));
        if (h->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }

        h->size = 0;
        h->max_size = 1;
    }
    else if (h->size == h->max_size) //time to increase available memory
    {
        h->max_size <<= 1;

        h->mem = realloc(h->mem, h->max_size * sizeof(void*));
        if (h->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }
    }

    return h->mem + (h->size++ * sizeof(void*));
}

void* mem_put_value(sort_data* const h, const void* const value)
{
    void* mem = NULL;

    //get next mem slot available in mem array
    mem = mem_get_free_slot(h);
    if (mem == NULL)
    {
        MSG("Error on get next available mem slot");
        return 0;
    }

    memcpy(mem, &value, sizeof(void*));

    return mem;
}
