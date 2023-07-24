#include "stage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "game.h"
#include "input.h"
#include "render.h"

Stage* stage_create(SDL_Renderer* renderer, Player* player) {
    Stage* self = (Stage*)calloc(1, sizeof(Stage));
    self->player = player;
    self->background = load_texture(renderer, "assets/images/purple_sky.png");

    return self;
}

void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window) {
    int window_height;
    int window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    const int scaled_window_width = (float)window_width /
                                    (float)RENDERER_SCALE_FACTOR;

    const int scaled_window_height = (float)window_height /
                                     (float)RENDERER_SCALE_FACTOR;

    const int rectX = (scaled_window_width - BACKGROUND_WIDTH) / 2;
    const int rectY = (scaled_window_height - BACKGROUND_HEIGHT) / 2;

    const SDL_Rect dstRect = {rectX, rectY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};

    SDL_RenderCopy(renderer, self->background, NULL, &dstRect);
}

void stage_update(Stage* self, InputEvent event) {
    player_update(self->player, event);

    switch (event) {
        default:
            break;
    }
}

void stage_destroy(Stage* self) {
    free(self);
}
