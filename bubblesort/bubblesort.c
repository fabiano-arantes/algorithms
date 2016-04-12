#include "bubblesort.h" 

#include <debug.h>

#include <stdlib.h>

int bubble_append(sort_data * const b, void* const value)
{
    if (b == NULL)
    {
        MSG("b NULL pointer");
        return 0;
    }

    if (b->cmp_func == NULL)
    {
        MSG("Comparison function not set");
        return 0;
    }

    pointer_t p = {value};

    //put new value at last position of array
    MEM_APPEND_VALUE(&b->mem, &p);

    ++b->size;

    //sort new value
    for (size_t i = b->size - 1; i > 0; --i)
    {
        size_t sizeof_pointer = sizeof(pointer_t) << 3;
        pointer_t *p1 = mem_get_value_address(&b->mem, i * sizeof_pointer, sizeof_pointer);
        pointer_t *p2 = mem_get_value_address(&b->mem, (i - 1) * sizeof_pointer, sizeof_pointer);
        const void* v1 = p1->pointer;
        const void* v2 = p2->pointer;

        if (b->cmp_func(v1, v2) == 0)
        {
            mem_exch(&b->mem, i, i - 1);
        }
        else
        {
            break;
        }
    }

    return b->size;
}

void* bubble_remove_top(sort_data * const b)
{
    void* ptop;

    if (b->size == 0)
    {
        MSG("array is empty");
        return NULL;
    }

    mem_seek(&b->mem, SEEK_READ_END);

    //get top value from memory
    if (MEM_GET_NEXT_VALUE(&b->mem, &ptop))
    {
        --b->size;

        if (mem_shrink(&b->mem, sizeof(void *) << 3))
        {
            return ptop;
        }
    }

    return NULL;
}
