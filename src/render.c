#include "render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, char* filename) {
    SDL_Texture* texture;
    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION,
        SDL_LOG_PRIORITY_INFO,
        "Loading %s",
        filename
    );
    texture = IMG_LoadTexture(renderer, filename);

    return texture;
}

void blit_sprite(
    SDL_Renderer* renderer, SDL_Texture* sprite_sheet, uint8_t row, uint8_t col,
    uint8_t dest_x, uint8_t dest_y, uint32_t w, uint32_t h
) {
    SDL_Rect src_rect = {col * w, row * h, w, h};
    SDL_Rect dest_rect = {0, 0, w, h};
    SDL_RenderCopy(renderer, sprite_sheet, &src_rect, &dest_rect);
}
