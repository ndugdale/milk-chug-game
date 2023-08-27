#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_NUM_FONTS 16

typedef struct {
    const char* id;
    TTF_Font* font;
} Font;

typedef struct {
    Font* fonts[MAX_NUM_FONTS];
    uint8_t insert_index;
} FontManager;

FontManager* font_manager_create(void);
TTF_Font* font_manager_get(FontManager* self, const char* id);
void font_manager_destroy(FontManager* self);

void blit_text(
    SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color colour,
    int64_t window_x, int64_t window_y
);