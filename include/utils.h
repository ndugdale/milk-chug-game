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

#define MAX_PATH_CHARS 512

void uint64_sort(uint64_t* values, size_t num_elements);
void get_asset_path(char* asset_path, const char* relative_path);
