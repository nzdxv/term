// def.h

#include <stdint.h>
#include <stdio.h>

// Debug only log macro
#ifdef _DEV
#define LOG(m, ...)                           \
    {                                         \
        printf("->> + " m "\n", __VA_ARGS__); \
    }  // MSVC only for now
#else
#define LOG(m, ...)
#endif
