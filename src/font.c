#include "font.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>

static void load_font(FontManager* self, const char* id, const char* path, uint8_t size);

FontManager* font_manager_create(void) {
    FontManager* self = calloc(1, sizeof(FontManager));
    load_font(self, "munro_10", "assets/fonts/MunroSmall.ttf", 10);

    return self;
}

TTF_Font* font_manager_get(FontManager* self, const char* id) {
    for (size_t i = 0; i < self->insert_index; i++) {
        if (strcmp(self->fonts[i]->id, id) == 0) {
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
    if (surface == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER,
            "Failed to create surface from text: %s",
            SDL_GetError()
        );
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (surface == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER,
            "Failed to create texture from text surface: %s",
            SDL_GetError()
        );
        return;
    }
    SDL_Rect rect = {window_x, window_y, surface->w, surface->h};

    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER,
            "Failed to render text: %s",
            SDL_GetError()
        );
    };

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

static void load_font(FontManager* self, const char* id, const char* path, uint8_t size) {
    if (self->insert_index > MAX_NUM_FONTS) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load font from file %s: "
            "font manager has run out of capacity",
            path
        );
        exit(1);
    }

    SDL_Log("Loading font: %s", path);
    TTF_Font* font = TTF_OpenFont(path, size);

    if (font == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load font from file %s: %s",
            path,
            TTF_GetError()
        );
        exit(1);
    }

    Font* insert_item = calloc(1, sizeof(Font));
    insert_item->id = id;
    insert_item->font = font;

    self->fonts[self->insert_index] = insert_item;
    self->insert_index++;
}
