#include "utils.h"

#include <SDL.h>
#include <stdint.h>
#include <stdlib.h>

static int uint64_compare(const void* a, const void* b);

void uint64_sort(uint64_t* values, size_t num_elements) {
    qsort(values, num_elements, sizeof(uint64_t), uint64_compare);
}

void get_asset_path(char* asset_path, const char* relative_path) {
    char path[MAX_PATH_CHARS];
    #ifdef PACKAGE_APP
        #ifdef __APPLE__
            snprintf(path, sizeof(path), "%s/%s", "../Resources/assets", relative_path);
        #elif __linux__
            snprintf(path, sizeof(path), "%s/%s", "../share/assets", relative_path);
        #endif
    #else
        snprintf(path, sizeof(path), "%s/%s", "../assets", relative_path);
    #endif
    strcpy(asset_path, path);
}

static int uint64_compare(const void* a, const void* b) {
    if (*(uint64_t*)a < *(uint64_t*)b) return -1;
    if (*(uint64_t*)a > *(uint64_t*)b) return 1;
    return 0;
}
