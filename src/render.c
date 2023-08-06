#include "render.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "utils.h"

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename) {
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

void blit_background(
    SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* background,
    double scale, uint32_t w, uint32_t h
) {
    uint32_t window_height;
    uint32_t window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    const uint32_t scaled_window_width =
        (double)window_width / scale;
    const uint32_t scaled_window_height =
        (double)window_height / scale;

    const uint32_t src_x = w >= scaled_window_width
        ? (w - scaled_window_width) / 2
        : 0;
    const uint32_t src_y = h >= scaled_window_height
        ? (h - scaled_window_height) / 2
        : 0;

    const uint32_t dst_x = scaled_window_width >= w
        ? (scaled_window_width - w) / 2
        : 0;
    const uint32_t dst_y = scaled_window_height >= h
        ? (scaled_window_height - h) / 2
        : 0;

    const SDL_Rect src_rect = {
        src_x, src_y, MIN(scaled_window_width, w),
        MIN(scaled_window_height, h)};
    const SDL_Rect dst_rect = {
        dst_x, dst_y, MIN(scaled_window_width, w),
        MIN(scaled_window_height, h)};

    SDL_RenderCopy(renderer, background, &src_rect, &dst_rect);
}

void blit_sprite(
    SDL_Renderer* renderer, SDL_Texture* sprite_sheet, uint8_t row, uint8_t col,
    uint8_t dest_x, uint8_t dest_y, uint32_t w, uint32_t h
) {
    const SDL_Rect src_rect = {col * w, row * h, w, h};
    const SDL_Rect dest_rect = {dest_x, dest_y, w, h};
    SDL_RenderCopy(renderer, sprite_sheet, &src_rect, &dest_rect);
}
