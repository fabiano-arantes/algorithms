#include <stdio.h>

#ifdef DEBUG
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
    #define __DEBUG_HEADER__ "DEBUG : %s:%s() (%d) : "
    #define MSG(x) printf(__DEBUG_HEADER__ x "\n", __FILENAME__, __FUNCTION__, __LINE__)
    #define MSG_ARG(x, ...) printf(__DEBUG_HEADER__ x "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
    #define MSG(x)
    #define MSG_ARG(x, ...)
#endif
