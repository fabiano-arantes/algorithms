#ifndef SORTDATA_H
#define SORTDATA_H

#include <debug.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct sort_data_
{
	void *mem;
	size_t size;
	size_t max_size;
	int (*cmp_func)(const void* const, const void* const);
}sort_data;

static inline void exch (sort_data *n, size_t i, size_t j)
{
    void *aux = malloc(sizeof(void*));
    memcpy(aux, n->mem + i * sizeof(void*), sizeof(void*));
    memcpy(n->mem + i * sizeof(void*), n->mem + j * sizeof(void*), sizeof(void*));
    memcpy(n->mem + j * sizeof(void*), aux, sizeof(void*));
    free(aux);
}

static inline void* get_obj_address(const sort_data* const n, int a)
{
    void *addr = n->mem + a * sizeof(void*);

    addr = (void *)*(typeof(void*)*)(addr);

    MSG_ARG("Object address = %p", addr);

    return addr;
}

static inline int asc_cmp_int(const void* const pv1, const void* const pv2)
{
    int v1 = *((int *)(pv1));
    int v2 = *((int *)(pv2));

    if (v1 < v2)
    {
        return 1;
    }

    return 0;
}

static inline int desc_cmp_int(const void* const pv1, const void* const pv2)
{
    int v1 = *((int *)(pv1));
    int v2 = *((int *)(pv2));

    if (v1 > v2)
    {
        return 1;
    }

    return 0;
}

void* mem_put_value(sort_data* const h, const void* const value);

void* mem_get_value(sort_data* const h, size_t index);

int mem_shrink(sort_data* const n);


#endif //SORTDATA_H
