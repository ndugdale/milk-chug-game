#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_NUM_TEXTURES 256

typedef struct {
    const char* id;
    SDL_Texture* texture;
} Texture;

typedef struct {
    Texture* textures[MAX_NUM_TEXTURES];
    uint8_t insert_index;
} TextureManager;

TextureManager* texture_manager_create(SDL_Renderer* renderer);
SDL_Texture* texture_manager_get(TextureManager* self, const char* id);
void texture_manager_destroy(TextureManager* self);
