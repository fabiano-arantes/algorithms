#ifndef MEM_H
#define MEM_H

typedef struct
{
    void *mem;
    size_t size;
    size_t max_size;
    size_t pos_r;
    size_t pos_w;
}mem;

typedef struct
{
    mem mem
    size_t pos_r;
    size_t pos_w;
}mem_bits;

#endif //MEM_H
