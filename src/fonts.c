#include "fonts.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>

FontManager* font_manager_create(void) {
    return calloc(1, sizeof(FontManager));
}

void font_manager_load(
    FontManager* self, const char* font_name, const char* font_path,
    uint8_t font_size
) {
    if (self->insert_index > MAX_NUM_FONTS) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load font from file %s: "
            "font manager has run out of capacity",
            font_path
        );
        exit(1);
    }

    TTF_Font* font = TTF_OpenFont(font_path, font_size);

    if (font == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load font from file %s: %s",
            font_path,
            SDL_GetError()
        );
        exit(1);
    }

    Font* insert_item = malloc(sizeof(Font));
    insert_item->font_name = font_name;
    insert_item->font = font;

    self->fonts[self->insert_index] = insert_item;
    self->insert_index++;
}

TTF_Font* font_manager_get(FontManager* self, const char* font_name) {
    for (size_t i = 0; i < self->insert_index; i++) {
        if (strcmp(self->fonts[i]->font_name, font_name) == 0) {
            return self->fonts[i]->font;
        }
    }
    return NULL;
}

void font_manager_destroy(FontManager* self) {
    for (size_t i = 0; i < self->insert_index; i++) {
        TTF_CloseFont(self->fonts[i]->font);
        free(self->fonts[i]);
    }
    free(self);
}

void blit_text(
    SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color colour,
    int64_t window_x, int64_t window_y
) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, colour);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect rect = {window_x, window_y, w, h};
    SDL_Log("Height: %d", h);

    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER,
            "Failed to render text: %s",
            SDL_GetError()
        );
    };
}
