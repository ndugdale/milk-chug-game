#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_NUM_FONTS 16

typedef enum {
    LeftAligned,
    CentreAligned,
    RightAligned
} TextAlignment;

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

// Blit a single line of text at window coordinates, between horizontal
// positions of window_x1 and window_x2.
// - text is truncated past the horizontal limit of window_x2
// - if window_x2 is equal to or less than window_x1, no truncation will occur
// - centre and right alignment will have no effect when window_x2 is equal to
//   or less than window_x1, or when truncation has occurred
void blit_text(
    SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color colour,
    int64_t window_x1, int64_t window_x2, int64_t window_y, TextAlignment alignment
);
