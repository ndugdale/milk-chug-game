#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX(a,b)                \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a > _b ? _a : _b;          \
})

#define MIN(a,b)                \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a < _b ? _a : _b;          \
})

void uint64_sort(uint64_t* values, size_t num_elements);
