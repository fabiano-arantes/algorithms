#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string.h>

#ifdef DEBUG
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
    #define __DEBUG_HEADER__ "DEBUG : %s:%s() (%d) : "
    #define MSG(x) printf(__DEBUG_HEADER__ x "\n", __FILENAME__, __FUNCTION__, __LINE__);
    #define MSG_ARG(x, ...) printf(__DEBUG_HEADER__ x "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__);
#else
    #define MSG(x)
    #define MSG_ARG(x, ...)
#endif

static inline void int_to_bits(int v, unsigned char bits_count, unsigned char bits[17])
{
    size_t i = 0;

    while (bits_count--)
    {
        bits[i++] = ((1 << bits_count) & v ? '1' : '0');
    }
    bits[i] = 0;
}

#endif //DEBUG_H
