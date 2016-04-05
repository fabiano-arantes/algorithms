#ifndef MEM_H
#define MEM_H

#include <debug.h>

#include <stdlib.h>

typedef unsigned long slot_t;

#define MEM_GET_NEXT_VALUE(mem, value) mem_get_next_bits(mem, value, sizeof(*value) << 3)
#define MEM_GET_NEXT_BIT(mem, value) mem_get_next_bits(mem, value, 1)
#define MEM_APPEND_VALUE(mem, value) mem_append_bits_and_shift(mem, (void *)value, sizeof(*value) << 3)
#define MEM_APPEND_BIT(mem, value) unsigned char v = value; mem_append_bits_and_shift(mem, &v, 1)

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
    slot_t *mem;
    size_t size;
    size_t max_size;
    size_t pos_r;
    unsigned short size_b;
    unsigned short pos_r_b;
}mem;

void* mem_copy_and_shift(mem* const m, const void* const value, size_t size_bytes);

int mem_shrink(mem* const n, size_t size_bits);

void mem_append_bits_and_shift(mem* const m, void* const bits, size_t size_bits);

int mem_get_next_bits(mem* const m, void* bits, size_t size);

void mem_seek(mem* const m, seek_t p);

static inline void mem_exch (mem *m, size_t i, size_t j)
{
    void *aux = malloc(sizeof(slot_t));
    memcpy(aux, m->mem + i, sizeof(slot_t));
    memcpy(m->mem + i, m->mem + j, sizeof(slot_t));
    memcpy(m->mem + j, aux, sizeof(slot_t));
    free(aux);
}

static inline size_t mem_size(const mem* const m)
{
    return m->size;
}

static inline size_t mem_size_in_bits(const mem* const m)
{
    return m->size * (sizeof(m->mem) << 3) + m->size_b;
}

void* mem_get_obj_address(const mem* const m, size_t index, size_t size);

static inline size_t mem_read_file(mem* const m, FILE* const fp, size_t size)
{
    void* mem = malloc(size);
    if (mem == NULL)
    {
        return 0;
    }

    size_t n = fread(mem, sizeof(void*), size, fp);

    mem_copy_and_shift(m, mem, size);

    free(mem);

    return n;
}

#endif //MEM_H
