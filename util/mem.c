#include "mem.h"

#include <limits.h>

const unsigned short SLOT_SIZE_BYTES = sizeof(slot_t);
const unsigned short SLOT_SIZE_BITS = SLOT_SIZE_BYTES << 3;
const slot_t SLOT_MASK = (slot_t)-1;

static void* mem_get_free_slot(mem* const m, size_t min_size_bits)
{
    if (m == NULL)
    {
        MSG("m is null");
        return NULL;
    }

    if ((m->used_slots > 0) && (m->bits_offset) && ((m->bits_offset + min_size_bits) < SLOT_SIZE_BITS))
    {
        m->bits_offset += min_size_bits;
        return m->mem + m->used_slots - 1;
    }

    size_t n, bits_offset, new_size_bits = 0, mem_pos = 0;

    if (m->used_slots > 0)
    {
        new_size_bits = (m->used_slots - 1) * SLOT_SIZE_BITS;
    }

    new_size_bits += min_size_bits + m->bits_offset;
    bits_offset = (new_size_bits % SLOT_SIZE_BITS);
    n = (new_size_bits / SLOT_SIZE_BITS) + 1; //((bits_offset  0) ? 1 : 0);

    if (m->used_slots > 0)
    {
        mem_pos = m->used_slots - 1;
    }
    m->used_slots = n;
    m->bits_offset = bits_offset;

    //check if mem was allocated
    if (m->mem == NULL)
    {
        if (m->used_slots == 0)
        {
            m->used_slots = 1;
        }

        m->max_slots = 1;

        while (m->max_slots <= m->used_slots)
        {
            m->max_slots <<= 1;
        }

        m->mem = malloc(m->max_slots * SLOT_SIZE_BYTES);
        if (m->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }

    }
    else if (m->used_slots >= m->max_slots)
    {
        while (m->max_slots <= m->used_slots)
        {
            m->max_slots <<= 1;
        }

        m->mem = realloc(m->mem, m->max_slots * SLOT_SIZE_BYTES);
        if (m->mem == NULL)
        {
            MSG("mem is NULL");
            return NULL;
        }
    }

    return m->mem + mem_pos;
}

int mem_get_last_bits(mem* const m, void* bits, size_t size_bits)
{
    size_t pos_bits = 0;

    if (m->used_slots > 0)
    {
        pos_bits = (m->used_slots - 1) * SLOT_SIZE_BITS;
    }

    pos_bits += m->bits_offset - size_bits;

    m->pos_r = pos_bits / SLOT_SIZE_BITS;
    m->pos_r_b =  pos_bits % SLOT_SIZE_BITS;

    return mem_get_bits(m, bits, size_bits);
}

int mem_get_next_bits(mem* const m, void* const bits, size_t size_bits)
{
    int ret = mem_get_bits(m, bits, size_bits);

    size_t pos_bits;

    pos_bits = m->pos_r * SLOT_SIZE_BITS + m->pos_r_b + size_bits;

    m->pos_r = pos_bits / SLOT_SIZE_BITS;
    m->pos_r_b = pos_bits % SLOT_SIZE_BITS;

    return ret;
}

int mem_get_bits(mem* const m, void* const bits, size_t size_bits)
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

    if ((m->pos_r >= m->used_slots) || ((m->pos_r == (m->used_slots - 1)) && ((m->pos_r_b + size_bits) > m->bits_offset)))
    {
        printf("Error: size_bits bigger than buffer\n");
        return 0;
    }

    slot_t *mem = m->mem + m->pos_r;
    if(mem == NULL)
    {
        printf("Error: mem is NULL\n");
        return 0;
    }

    slot_t *bits_ = bits;
    slot_t mask;

    if (m->pos_r_b > 0)
    {
        while (size_bits > 0)
        {
            if (size_bits >= SLOT_SIZE_BITS)
            {
                mask = SLOT_MASK;
            }
            else
            {
                mask = ~(SLOT_MASK << size_bits);
            }

            *bits_ &= 0; //clean destination
            *bits_ |= mask & (*mem >> m->pos_r_b);


            if ((m->pos_r_b + size_bits) > SLOT_SIZE_BITS)
            {
                ++mem;
                *bits_ |= mask & (*mem << (SLOT_SIZE_BITS - m->pos_r_b));

                if (size_bits >= SLOT_SIZE_BITS)
                {
                    size_bits -= SLOT_SIZE_BITS;
                }
                else
                {
                    break;
                }

                ++bits_;
            }
            else
            {
                *bits_ &= ~(SLOT_MASK << size_bits); //clean most significant bits
                break;
            }
        }
    }
    else
    {
        size_t bits_offset = size_bits % (1 << 3);
        size_t n = (size_bits >> 3); //full bytes

        if (n > 0)
        {
            memmove(bits, mem, n);
        }

        if (bits_offset > 0)
        {
            unsigned char *last_byte = ((unsigned char *)mem) + n;
            unsigned char *last_bits = ((unsigned char *)bits) + n;

            *last_bits &= 0; //clean bits
            *last_bits |= ~(UCHAR_MAX << bits_offset) & *last_byte;
        }
    }

    return 1;
}

int mem_get_bits_at(mem* const m, size_t position_bits, void* const bits, size_t size_bits)
{
    m->pos_r = position_bits / SLOT_SIZE_BITS;
    m->pos_r_b =  position_bits % SLOT_SIZE_BITS;

    return mem_get_bits(m, bits, size_bits);
}

void mem_append_bits_and_shift(mem* const m, void* const bits, size_t size_bits)
{
    slot_t *mem = NULL;
    slot_t *bits_ = NULL;
    size_t shift_before = 0;

    slot_t bits_mask;

    shift_before = m->bits_offset;
    bits_ = bits;

    mem = mem_get_free_slot(m, size_bits);

    if (shift_before > 0)
    {
        while (size_bits > 0)
        {
            if (size_bits >= SLOT_SIZE_BITS)
            {
                bits_mask = SLOT_MASK;
            }
            else
            {
                bits_mask = ~(SLOT_MASK << size_bits);
            }

            *mem &= ~(bits_mask << shift_before); //clear bits
            *mem |= (bits_mask & *bits_) << shift_before;

            ++mem;

            if ((shift_before + size_bits) > SLOT_SIZE_BITS)
            {
                *mem &= ~(bits_mask >> (SLOT_SIZE_BITS - shift_before)); //clear bits
                *mem |= (bits_mask & *bits_) >> (SLOT_SIZE_BITS - shift_before);
            }

            if (size_bits > SLOT_SIZE_BITS)
            {
                ++bits_;
                size_bits -= SLOT_SIZE_BITS;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        size_t n = size_bits >> 3;
        size_t bits_offset = size_bits % (1 << 3);

        if (n > 0)
        {
            memmove(mem, bits, n);
        }

        if (bits_offset > 0)
        {
            unsigned char *last_byte = ((unsigned char *)mem) + n;
            unsigned char *last_bits = ((unsigned char *)bits) + n;

            *last_byte &= ~(UCHAR_MAX << bits_offset); //clean bits
            *last_byte |= *last_bits;
        }
    }
}

int mem_shrink(mem* const m, size_t size_bits)
{
    if (m->max_slots == 0)
    {
        MSG("mem is empty");
        return 0;
    }

    if (size_bits <= m->bits_offset)
    {
        m->bits_offset -= size_bits;

        return 1;
    }

    size_t n_bits = ((m->used_slots - 1) * SLOT_SIZE_BITS) + m->bits_offset;

    if (size_bits > n_bits)
    {
        printf("Error: size_bits bigger the allocated bits\n");

        return 0;
    }


    if (size_bits <= m->bits_offset)
    {
        m->bits_offset -= size_bits;
        return 1;
    }
    else if ((size_bits > m->bits_offset) && (size_bits <= SLOT_SIZE_BITS))
    {
        m->bits_offset = SLOT_SIZE_BITS + m->bits_offset - size_bits;
        --m->used_slots;
    }
    else if (size_bits > SLOT_SIZE_BITS)
    {
        size_t n = (size_bits + m->bits_offset) / SLOT_SIZE_BITS;
        size_t shift = (size_bits + m->bits_offset) % SLOT_SIZE_BITS;

        m->used_slots -= n;
        m->bits_offset = shift;
    }

    if ((m->used_slots <= m->pos_r) || ((m->used_slots == (m->pos_r - 1)) && (m->bits_offset < m->pos_r_b)))
    {
        m->pos_r = m->used_slots - 1;
        m->pos_r_b = m->bits_offset;
    }

    if ((m->max_slots > 0) && (m->used_slots <= (m->max_slots >> 2)))
    {
        m->max_slots >>= 1;

        if ((m->used_slots > 0) && (m->max_slots > 0))
        {
            m->mem = realloc(m->mem, m->max_slots * SLOT_SIZE_BYTES);

            if (m->mem == NULL)
            {
                MSG("Error on reallocate memory");

                m->max_slots = 0;
                m->used_slots = 0;
                m->pos_r = 0;
                m->pos_r_b = 0;
                m->bits_offset = 0;

                return 0;
            }
        }
        else
        {
            free(m->mem);
            m->mem = NULL;
            m->max_slots = 0;
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
            m->pos_r = m->used_slots - 1;

            if(m->bits_offset > 0)
            {
                m->pos_r_b = m->bits_offset - 1;
            }
            break;
    }
}

size_t mem_size(const mem* const m)
{
    size_t n_bits = mem_size_in_bits(m);
    size_t bits_offset = n_bits % (1 << 3);

    return mem_size_in_bits(m) / (1 << 3) + ((bits_offset > 0) ? 1 : 0);
}

size_t mem_size_in_bits(const mem* const m)
{
    if (m->used_slots == 0)
    {
        return 0;
    }

    return (m->used_slots - 1) * SLOT_SIZE_BITS + m->bits_offset;
}
