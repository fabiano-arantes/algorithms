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

    if (b->cmp_func == NULL)
    {
        MSG("Comparison function not set");
        return 0;
    }

    //put new value at last position of array
    if (mem_put_value(b, value) == NULL)
    {
        MSG("Error on put new value");
        return 0;
    }

    //sort new value
    for (unsigned int i = b->size - 1; i > 0; --i)
    {
        if (b->cmp_func(get_obj_address(b, i), get_obj_address(b, i - 1)) == 0)
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

void* bubble_remove_top(sort_data * const b)
{
    void* top;

    if (b->size == 0)
    {
        MSG("array is empty");
        return NULL;
    }

    //get top value from memory
    if ((top = mem_get_value(b, b->size - 1)) != NULL)
    {
        if (mem_shrink(b))
        {
            return top;
        }
    }

    return NULL;
}
