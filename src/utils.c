#include "utils.h"

#include <stdint.h>
#include <stdlib.h>

static int uint64_compare(const void* a, const void* b);

void uint64_sort(uint64_t* values, size_t num_elements) {
    qsort(values, num_elements, sizeof(uint64_t), uint64_compare);
}

static int uint64_compare(const void* a, const void* b) {
    if (*(uint64_t*)a < *(uint64_t*)b) return -1;
    if (*(uint64_t*)a > *(uint64_t*)b) return 1;
    return 0;
}
