#ifndef MEM_H
#define MEM_H

#include <debug.h>

#include <stdlib.h>

typedef unsigned long slot_t;

#define MEM_APPEND_VALUE(mem, value) mem_append_bits_and_shift(mem, (void *)value, sizeof(*value) << 3)
#define MEM_APPEND_BIT(mem, value) temp__ = value; mem_append_bits_and_shift(mem, &temp__, 1)
#define MEM_GET_LAST_VALUE(mem, value) mem_get_last_bits(mem, value, sizeof(*value) << 3)
#define MEM_GET_LAST_BIT(mem, value) mem_get_last_bits(mem, value, 1)
#define MEM_GET_NEXT_VALUE(mem, value) mem_get_next_bits(mem, value, sizeof(*value) << 3)
#define MEM_GET_NEXT_BIT(mem, value) mem_get_next_bits(mem, value, 1)

static slot_t temp__;

typedef enum
{
    BIT_0 = 0,
    BIT_1
}bit_t;

typedef enum
{
    SEEK_READ_RESET,
    SEEK_READ_END
}seek_t;

typedef struct
{
    const void* const pointer;
}pointer_t;

typedef struct
{
    slot_t *mem;
    size_t used_slots;
    size_t max_slots;
    size_t pos_r;
    unsigned short bits_offset;
    unsigned short pos_r_b;
}mem;

void mem_append_bits_and_shift(mem* const m, void* const bits, size_t size_bits);

int mem_get_bits(mem* const m, void* const bits, size_t size_bits);

int mem_get_bits_at(mem* const m, size_t position_bits, void* const bits, size_t size_bits);

int mem_get_last_bits(mem* const m, void* bits, size_t size_bits);

int mem_get_next_bits(mem* const m, void* const bits, size_t size_bits);

void mem_seek(mem* const m, seek_t p);

int mem_shrink(mem* const n, size_t size_bits);

size_t mem_size(const mem* const m);

size_t mem_size_in_bits(const mem* const m);


static inline void mem_exch (mem *m, size_t i, size_t j)
{
    if (i == j)
    {
        return;
    }

    slot_t aux;
    memmove(&aux, m->mem + i, sizeof(slot_t));
    memmove(m->mem + i, m->mem + j, sizeof(slot_t));
    memmove(m->mem + j, &aux, sizeof(slot_t));
}

static inline size_t mem_read_file(mem* const m, FILE* const fp, size_t size)
{
    void* mem = malloc(size);
    if (mem == NULL)
    {
        return 0;
    }

    size_t n = fread(mem, 1, size, fp);
    if (n != size)
    {
        printf("Error: amount of bytes read from file smaller than expected");

        return 0;
    }

    mem_append_bits_and_shift(m, mem, size << 3);

    free(mem);

    return n;
}

#endif //MEM_H
