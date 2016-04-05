#include "mem.h"

#include <limits.h>

const unsigned short SLOT_SIZE_BYTES = sizeof(slot_t);
const unsigned short SLOT_SIZE_BITS = SLOT_SIZE_BYTES << 3;
const unsigned short SLOT_MASK = (slot_t) ~(1 << SLOT_SIZE_BITS);

static void* mem_get_free_slot(mem* const m);

void* mem_copy_and_shift(mem* const m, const void* const value, size_t size_bytes)
{
    unsigned short times = (size_bytes / SLOT_SIZE_BYTES) + (size_bytes % SLOT_SIZE_BYTES ? 1 : 0);
    void* mem = NULL;

    for (unsigned short i = 0; i < times; ++i)
    {
        mem = mem_get_free_slot(m);
        memcpy(mem, value + i * SLOT_SIZE_BYTES, SLOT_SIZE_BYTES);
    }

    return mem;
}

static void* mem_get_free_slot(mem* const m)
{
    if (m == NULL)
    {
        MSG("m is null");
        return NULL;
    }

    if ((m->size > 0) && (m->size_b < SLOT_SIZE_BITS))
    {
        return m->mem + (m->size - 1) * SLOT_SIZE_BYTES;
    }

    m->size_b = 0;

    //check if mem was allocated
    if (m->mem == NULL)
    {
        m->mem = malloc(SLOT_SIZE_BYTES << 1);
        if (m->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }

        m->size = 0;
        m->max_size = 2;
    }
    else if (m->size == m->max_size) //time to increase available memory
    {
        m->max_size <<= 1;

        m->mem = realloc(m->mem, m->max_size * SLOT_SIZE_BYTES);
        if (m->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }
    }

    return m->mem + m->size++;
}

void* mem_get_obj_address(const mem* const m, size_t index, size_t size)
{
    size_t n = (size * index) / SLOT_SIZE_BYTES;

    if (n >= m->size)
    {
        printf("Error: index out of range");
        return NULL;
    }

    size_t shift = (size * index) % SLOT_SIZE_BYTES;

    void *addr = ((void *)(m->mem + n)) + shift;

    //MSG_ARG("Object address = %p", addr);

    return addr;
}

static slot_t* mem_get_slot(mem* const m, size_t index)
{
    if (m == NULL)
    {
        MSG("node is NULL");
        return 0;
    }

    if (m->mem == NULL)
    {
        MSG("mem is NULL");
        return 0;
    }

    return mem_get_obj_address(m, index, sizeof(slot_t));
}

int mem_get_next_bits(mem* const m, void* bits, size_t size)
{
    if (m == NULL)
    {
        printf("Error: mem is NULL\n");
        return 0;
    }

    if (bits == NULL)
    {
        printf("Error: bits is NULL\n");
        return 0;
    }

    if ((m->pos_r == (m->size - 1)) && (m->pos_r_b + size) > m->size_b)
    {
        printf("Error: size bigger than buffer\n");
        return 0;
    }

    slot_t *mem;
    slot_t *bits_ = bits;

    while (size > 0)
    {
        mem = mem_get_slot(m, m->pos_r);

        if(mem == NULL)
        {
            printf("Error: mem is NULL\n");
            return 0;
        }

        if((size_t)(m->pos_r_b + size) < SLOT_SIZE_BITS)
        {
            *bits_ |= *mem << (SLOT_SIZE_BITS - size - m->pos_r_b);
            m->pos_r_b += size;

            break;
        }
        else
        {
            *bits_ |= *mem >> (m->pos_r_b + size - SLOT_SIZE_BITS);

             m->pos_r_b = 0;
             ++m->pos_r;

             size -= SLOT_SIZE_BITS;
        }
    }

    return 1;
}

void mem_append_bits_and_shift(mem* const m, void* const bits, size_t size_bits)
{
    slot_t *buf = NULL;
    slot_t *bits_ = bits;

    while (size_bits > 0)
    {
        buf = mem_get_free_slot(m);

        if ((size_t)(m->size_b + size_bits) < SLOT_SIZE_BITS)
        {
            *buf &= ~SLOT_MASK << (SLOT_SIZE_BITS - size_bits - m->size_b);
            *buf |= *bits_ << (SLOT_SIZE_BITS - size_bits - m->size_b);
            m->size_b += size_bits;

            break;
        }
        else
        {
            *buf &= ~SLOT_MASK >> (m->size_b + size_bits - SLOT_SIZE_BITS);
            *buf |= *bits_ >> (m->size_b + size_bits - SLOT_SIZE_BITS);

            m->size_b = SLOT_SIZE_BITS;

            size_bits -= SLOT_SIZE_BITS;
        }
    }
}

int mem_shrink(mem* const m, size_t size_bits)
{
    if (m->max_size == 0)
    {
        MSG("mem is empty");
        return 0;
    }

    if (size_bits <= m->size_b)
    {
        m->size_b -= size_bits;

        return 1;
    }

    size_t n_bits = ((m->size - 1) * (sizeof(slot_t) << 3)) + m->size_b;

    if (size_bits > n_bits)
    {
        printf("Error: size_bits bigger the allocated bits\n");

        return 0;
    }

    size_t n = size_bits / SLOT_SIZE_BITS;
    size_t shift = size_bits % SLOT_SIZE_BITS;

    if (shift > m->size_b)
    {
        m->size_b = m->size_b + SLOT_SIZE_BITS - shift;
        --m->size;
    }
    else
    {
        m->size_b -= shift;
    }

    m->size -= n;

    if (m->size <= m->pos_r)
    {
        m->pos_r = m->size - 1;
        m->pos_r_b = 0;
    }

    if ((m->pos_r == (m->size - 1)) && (m->size_b < m->pos_r_b))
    {
        m->pos_r_b = m->size_b;
    }

    if ((m->max_size > 0) && (m->size <= (m->max_size >> 2)))
    {
        m->max_size >>= 1;

        if ((m->size > 0) && (m->max_size > 0))
        {
            m->mem = realloc(m->mem, m->max_size * SLOT_SIZE_BYTES);

            if (m->mem == NULL)
            {
                MSG("Error on reallocate memory");

                m->max_size = 0;
                m->size = 0;
                m->pos_r = 0;
                m->pos_r_b = 0;
                m->size_b = 0;

                return 0;
            }
        }
        else
        {
            free(m->mem);
            m->mem = NULL;
            m->max_size = 0;
        }
    }

    return 1;
}

void mem_seek(mem* const m, seek_t p)
{
    switch(p)
    {
        case SEEK_READ_RESET:
            m->pos_r = 0;
            m->pos_r_b = 0;
            break;
        case SEEK_READ_END:
            m->pos_r = m->size - 1;
            m->pos_r_b = m->size_b - 1;
            break;
    }
}
