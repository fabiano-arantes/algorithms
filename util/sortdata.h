#ifndef SORTDATA_H
#define SORTDATA_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct sort_data_
{
	void *mem;
	size_t type_size;
	size_t size;
	size_t max_size;
	int (*cmp_func)(const struct sort_data_* const, int, int);
}sort_data;

static inline void exch (sort_data *n, size_t i, size_t j)
{
    void *aux = malloc(n->type_size);
    memcpy(aux, n->mem + i * n->type_size, n->type_size);
    memcpy(n->mem + i * n->type_size, n->mem + j * n->type_size, n->type_size);
    memcpy(n->mem + j * n->type_size, aux, n->type_size);
    free(aux);
}

static inline int asc_cmp_int(const sort_data* const n, int a, int b)
{
    a = *((int *)(n->mem) + a);
    b = *((int *)(n->mem) + b);

    if (a < b)
    {
        return 1;
    }

    return 0;
}

static inline int desc_cmp_int(const sort_data* const n, int a, int b)
{
    a = *((int *)(n->mem) + a);
    b = *((int *)(n->mem) + b);

    if (a > b)
    {
        return 1;
    }

    return 0;
}

void* mem_put_value(sort_data* const h, const void* const value);

int mem_get_value(sort_data* const h, size_t index, void* const value);

void* mem_shrink(sort_data* const n);


#endif //SORTDATA_H
