#include "bubblesort.h" 

#include <debug.h>

#include <stdlib.h>

int bubble_append(sort_data * const b, const void* const value)
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
    if (mem_append_value(&b->mem, &value, sizeof(void*)) == NULL)
    {
        MSG("Error on put new value");
        return 0;
    }

    void * v1;
    void * v2;

    //sort new value
    for (size_t i = mem_size(&b->mem) - 1; i > 0; --i)
    {
        v1 = *(void **)mem_get_obj_address(&b->mem, i, sizeof(void *));
        v2 = *(void **)mem_get_obj_address(&b->mem, i - 1, sizeof(void *));

        if (b->cmp_func(v1, v2) == 0)
        {
            mem_exch(&b->mem, i, i - 1);
        }
        else
        {
            break;
        }
    }

    return mem_size(&b->mem);
}

void* bubble_remove_top(sort_data * const b)
{
    void* top;

    if (mem_size(&b->mem) == 0)
    {
        MSG("array is empty");
        return NULL;
    }

    //get top value from memory
    if ((top = mem_get_value(&b->mem, mem_size(&b->mem) - 1)) != NULL)
    {
        if (mem_shrink(&b->mem))
        {
            return top;
        }
    }

    return NULL;
}
