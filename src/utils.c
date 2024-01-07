#include "utils.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>

static int uint64_compare(const void* a, const void* b);

void uint64_sort(uint64_t* values, size_t num_elements) {
    qsort(values, num_elements, sizeof(uint64_t), uint64_compare);
}

void get_asset_path(char* asset_path, const char* relative_path) {
    char path[MAX_PATH_CHARS];
    const char* env = getenv("APPDIR");
    const char* appdir = (env != NULL) ? env : ".";
    const char* assets_path = (env != NULL) ? "usr/share/assets" : "../assets";
    snprintf(path, sizeof(path), "%s/%s/%s", appdir, assets_path, relative_path);
    strcpy(asset_path, path);
}

static int uint64_compare(const void* a, const void* b) {
    if (*(uint64_t*)a < *(uint64_t*)b) return -1;
    if (*(uint64_t*)a > *(uint64_t*)b) return 1;
    return 0;
}
