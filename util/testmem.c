#include "mem.h"

//#include <stdio.h>

static mem bits;
static size_t size = 0;

#define CHECK_STR_BITS(value, expected_value, line)\
{\
    if (memcmp(value, expected_value, sizeof(*value)) != 0)\
    {\
        printf("[line %u] Error: structs of different contents\n", line);\
    }\
}\

#define CHECK_VALUE_BITS(value, expected_value, line)\
{\
    if (*value != expected_value)\
    {\
        printf("[line %u] Error: expected value = 0x%lX, found value: 0x%lX\n", line, (unsigned long)expected_value, (unsigned long)*value);\
    }\
}\

void check_size_bits(size_t expected_value, unsigned int line)
{
    if (mem_size_in_bits(&bits) != expected_value)
    {
        printf("[line %u] Error: expected size in bits is %zu bit(s). Found bits: %zu\n", line, expected_value, mem_size_in_bits(&bits));
    }
}

#define APPEND_STR_AND_CHECK(value)\
    MEM_APPEND_VALUE(&bits, value);\
    size += sizeof(*value) << 3; \
    check_size_bits(size, __LINE__);\

#define APPEND_VALUE_AND_CHECK(value)\
    MEM_APPEND_VALUE(&bits, value);\
    size += sizeof(*value) << 3; \
    check_size_bits(size, __LINE__);\

#define APPEND_BIT_AND_CHECK(value)\
    MEM_APPEND_BIT(&bits, value);\
    size += 1; \
    check_size_bits(size, __LINE__);\

#define GET_NEXT_STR_AND_CHECK(value, expected_value)\
    if (MEM_GET_NEXT_VALUE(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_NEXT_VALUE failed\n", __LINE__);\
    }\
    CHECK_STR_BITS(value, expected_value, __LINE__)\

#define GET_NEXT_VALUE_AND_CHECK(value, expected_value)\
    if (MEM_GET_NEXT_VALUE(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_NEXT_VALUE failed\n", __LINE__);\
    }\
    CHECK_VALUE_BITS(value, expected_value, __LINE__)\

#define GET_NEXT_BIT_AND_CHECK(value, expected_value)\
    if (MEM_GET_NEXT_BIT(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_NEXT_BIT failed\n", __LINE__);\
    }\
    CHECK_VALUE_BITS(value, expected_value, __LINE__)\

#define POP_LAST_STR_AND_CHECK(value, expected_value)\
    if(MEM_GET_LAST_VALUE(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_LAST_VALUE failed\n", __LINE__);\
    }\
    CHECK_STR_BITS(value, expected_value, __LINE__)\
    mem_shrink(&bits, sizeof(*value) << 3);\
    size -= sizeof(*value) << 3; \
    check_size_bits(size, __LINE__);\

#define POP_LAST_VALUE_AND_CHECK(value, expected_value)\
    if(MEM_GET_LAST_VALUE(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_LAST_VALUE failed\n", __LINE__);\
    }\
    CHECK_VALUE_BITS(value, expected_value, __LINE__)\
    mem_shrink(&bits, sizeof(*value) << 3);\
    size -= sizeof(*value) << 3; \
    check_size_bits(size, __LINE__);\

#define POP_LAST_BIT_AND_CHECK(value, expected_value)\
    if (MEM_GET_LAST_BIT(&bits, value) == 0)\
    {\
        printf("[%u] MEM_GET_LAST_BIT failed\n", __LINE__);\
    }\
    CHECK_VALUE_BITS(value, expected_value, __LINE__)\
    mem_shrink(&bits, 1);\
    size -= 1;\
    check_size_bits(size, __LINE__);\

struct ulong_ulong
{
    unsigned long a;
    unsigned long b;
};

unsigned char a = 0xFE;
unsigned short s = 0xDCBA;
unsigned int i = 0x1B3D5F7;
unsigned long l = 0x2A2A4B4B6C6C8E8E;
struct ulong_ulong ulul = {0xFEDCBA9876543210,0x0123456789ABCDEF};

bit_t rb = 1;
unsigned char ra = 0;
unsigned short rs = 0;
unsigned int ri = 0;
unsigned long rl = 0;
struct ulong_ulong rulul = {0, 0};

void test_one_by_one(void)
{
    APPEND_BIT_AND_CHECK(BIT_1); //1 bit
    mem_seek(&bits, SEEK_READ_RESET);
    rb = BIT_1;
    GET_NEXT_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    POP_LAST_BIT_AND_CHECK(&rb, BIT_1); //1 bit

    APPEND_VALUE_AND_CHECK(&a); //8 bits
    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_VALUE_AND_CHECK(&ra, a); //8 bits
    POP_LAST_VALUE_AND_CHECK(&ra, a); //8 bits

    APPEND_VALUE_AND_CHECK(&s); //16 bits
    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_VALUE_AND_CHECK(&rs, s); //16 bits
    POP_LAST_VALUE_AND_CHECK(&rs, s); //16 bits

    APPEND_VALUE_AND_CHECK(&i); //32 bits
    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_VALUE_AND_CHECK(&ri, i); //32 bits
    POP_LAST_VALUE_AND_CHECK(&ri, i); //32 bits

    APPEND_VALUE_AND_CHECK(&l); //32/64 bits
    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_VALUE_AND_CHECK(&rl, l); //32/64 bits
    POP_LAST_VALUE_AND_CHECK(&rl, l); //32/64 bits

    APPEND_STR_AND_CHECK(&ulul); //64/128 bits
    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_STR_AND_CHECK(&rulul, &ulul); //64/128 bits
    POP_LAST_STR_AND_CHECK(&rulul, &ulul); //64/128 bits
}

void test_sorted(void)
{
    rb = BIT_1;

    APPEND_BIT_AND_CHECK(BIT_1); //1 bit
    APPEND_VALUE_AND_CHECK(&a); //8 bits
    APPEND_VALUE_AND_CHECK(&s); //16 bits
    APPEND_VALUE_AND_CHECK(&i); //32 bits
    APPEND_VALUE_AND_CHECK(&l); //32/64 bits
    APPEND_STR_AND_CHECK(&ulul); //64/128 bits

    //-------------------------------------------------------------------------

    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    GET_NEXT_VALUE_AND_CHECK(&ra, a); //8 bits
    GET_NEXT_VALUE_AND_CHECK(&rs, s); //16 bits
    GET_NEXT_VALUE_AND_CHECK(&ri, i); //32 bits
    GET_NEXT_VALUE_AND_CHECK(&rl, l); //32/64 bits
    GET_NEXT_STR_AND_CHECK(&rulul, &ulul); //64/128 bits

    //-------------------------------------------------------------------------

    POP_LAST_STR_AND_CHECK(&rulul, &ulul); //64/128 bits
    POP_LAST_VALUE_AND_CHECK(&rl, l); //32/64 bits
    POP_LAST_VALUE_AND_CHECK(&ri, i); //32 bits
    POP_LAST_VALUE_AND_CHECK(&rs, s); //16 bits
    POP_LAST_VALUE_AND_CHECK(&ra, a); //8 bits
    POP_LAST_BIT_AND_CHECK(&rb, BIT_1); //1 bit
}

void test_random(void)
{
    APPEND_VALUE_AND_CHECK(&l); //32/64 bits
    APPEND_BIT_AND_CHECK(BIT_1); //1 bit
    APPEND_VALUE_AND_CHECK(&a); //8 bits
    APPEND_STR_AND_CHECK(&ulul); //64/128 bits
    APPEND_VALUE_AND_CHECK(&s); //16 bits
    APPEND_VALUE_AND_CHECK(&i); //32 bits
    APPEND_BIT_AND_CHECK(BIT_0); //1 bit
    APPEND_VALUE_AND_CHECK(&s); //16 bits

    //-------------------------------------------------------------------------

    mem_seek(&bits, SEEK_READ_RESET);
    GET_NEXT_VALUE_AND_CHECK(&rl, l); //32/64 bits
    rb = BIT_1;
    GET_NEXT_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    GET_NEXT_VALUE_AND_CHECK(&ra, a); //8 bits
    GET_NEXT_STR_AND_CHECK(&rulul, &ulul); //64/128 bits
    GET_NEXT_VALUE_AND_CHECK(&rs, s); //16 bits
    GET_NEXT_VALUE_AND_CHECK(&ri, i); //32 bits
    rb = BIT_0;
    GET_NEXT_BIT_AND_CHECK(&rb, BIT_0); //1 bit
    GET_NEXT_VALUE_AND_CHECK(&rs, s); //16 bits

    //-------------------------------------------------------------------------

    POP_LAST_VALUE_AND_CHECK(&rs, s); //16 bits
    rb = BIT_0;
    POP_LAST_BIT_AND_CHECK(&rb, BIT_0); //1 bit
    POP_LAST_VALUE_AND_CHECK(&ri, i); //32 bits
    POP_LAST_VALUE_AND_CHECK(&rs, s); //16 bits
    POP_LAST_STR_AND_CHECK(&rulul, &ulul); //64/128 bits
    POP_LAST_VALUE_AND_CHECK(&ra, a); //8 bits
    rb = BIT_1;
    POP_LAST_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    POP_LAST_VALUE_AND_CHECK(&rl, l); //64 bits
}

void test_over_flow(void)
{
    APPEND_VALUE_AND_CHECK(&l); //32/64 bits
    APPEND_BIT_AND_CHECK(BIT_1); //1 bit

    GET_NEXT_VALUE_AND_CHECK(&rl, l); //32/64 bits
    rb = BIT_1;
    GET_NEXT_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    GET_NEXT_VALUE_AND_CHECK(&ra, a); //8 bits -- must generate error

    rb = BIT_1;
    POP_LAST_BIT_AND_CHECK(&rb, BIT_1); //1 bit
    POP_LAST_VALUE_AND_CHECK(&rl, l); //64 bits
    rb = BIT_0;
    POP_LAST_BIT_AND_CHECK(&rb, BIT_0); //1 bit -- must generate error
}

int main()
{
    memset(&bits, 0, sizeof(bits));

    test_one_by_one();

    test_sorted();

    test_random();

    test_over_flow();

    return 0;
}
