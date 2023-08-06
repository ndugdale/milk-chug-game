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
    int64_t window_x, int64_t window_y, uint32_t w, uint32_t h
) {
    uint32_t dst_x;
    uint32_t dst_y;
    uint32_t src_x;
    uint32_t src_y;
    uint32_t rect_w = 0;
    uint32_t rect_h = 0;

    if (window_x >= 0) {
        dst_x = window_x;
        src_x = 0;
        rect_w = w;
    } else if (window_x + (int64_t)w >= 0) {
        dst_x = 0;
        src_x = -window_x;
        rect_w = w + window_x;
    }

    if (window_y >= 0) {
        dst_y = window_y;
        src_y = 0;
        rect_h = h;
    } else if (window_y + (int64_t)h >= 0) {
        dst_y = 0;
        src_y = -window_y;
        rect_h = h + window_y;
    }

    if (rect_w > 0 && rect_h > 0) {
        const SDL_Rect src_rect = {
            col * w + src_x,
            row * h + src_y,
            rect_w,
            rect_h};

        const SDL_Rect dest_rect = {
            dst_x,
            dst_y,
            rect_w,
            rect_h};

        SDL_RenderCopy(renderer, sprite_sheet, &src_rect, &dest_rect);
    }
}

void background_xy_to_window_xy(
    SDL_Window* window, int64_t background_x, int64_t background_y,
    uint32_t background_w, uint32_t background_h,
    int64_t* window_x, int64_t* window_y
) {
    uint32_t window_height;
    uint32_t window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    const int64_t scaled_window_width =
        (double)window_width / (double)RENDERER_SCALE_FACTOR;
    const int64_t scaled_window_height =
        (double)window_height / (double)RENDERER_SCALE_FACTOR;

    *window_x = background_x + (scaled_window_width - background_w) / 2;
    *window_y = background_y + (scaled_window_height - background_h) / 2;
}
