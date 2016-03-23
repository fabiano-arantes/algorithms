#include "bubblesort.h" 

#include <debug.h>

#include <stdlib.h>

int bubble_put(sort_data * const b, const void* const value)
{
    if (b == NULL)
    {
        MSG("b NULL pointer");
        return 0;
    }

    //put new value at last position of array
    if (mem_put_value(b, value) == NULL)
    {
        MSG("Error on put new value");
        return 0;
    }

    for (unsigned int i = b->size - 1; i > 0; --i)
    {
        if (b->cmp_func(b, i, i - 1) == 0)
        {
            exch(b, i, i - 1);
        }
        else
        {
            break;
        }
    }

    return b->size;
}

int bubble_remove_top(sort_data * const b, void* const top)
{
    //get top value from memory
    if (mem_get_value(b, b->size - 1, top))
    {
        return (mem_shrink(b) != NULL);
    }

    return 0;
}
