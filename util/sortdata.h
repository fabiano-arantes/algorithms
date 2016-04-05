#ifndef SORTDATA_H
#define SORTDATA_H

#include <debug.h>
#include <mem.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct sort_data_
{
    mem mem;
    size_t size;
    int (*cmp_func)(const void* const, const void* const);
}sort_data;

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

#endif //SORTDATA_H
