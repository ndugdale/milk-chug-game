#include "stage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>

#include "events.h"
#include "game.h"
#include "opponent.h"
#include "player.h"
#include "render.h"

static bool opponents_are_finished(Opponent* const* opponents);

Stage* stage_create(SDL_Renderer* renderer, Player* player) {
    Stage* self = (Stage*)calloc(1, sizeof(Stage));
    self->background = load_texture(renderer, "assets/images/purple_sky.png");
    self->player = player;

    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        self->opponents[i] = opponent_create();
    }

    return self;
}

void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window) {
    uint32_t window_height;
    uint32_t window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    const uint32_t scaled_window_width = (float)window_width /
                                         (float)RENDERER_SCALE_FACTOR;

    const uint32_t scaled_window_height = (float)window_height /
                                          (float)RENDERER_SCALE_FACTOR;

    const uint32_t rectX = (scaled_window_width - BACKGROUND_WIDTH) / 2;
    const uint32_t rectY = (scaled_window_height - BACKGROUND_HEIGHT) / 2;

    const SDL_Rect dstRect = {rectX, rectY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};

    SDL_RenderCopy(renderer, self->background, NULL, &dstRect);
}

void stage_update(Stage* self, Event event) {
    player_update(self->player, event);

    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_update(self->opponents[i], event);
    }

    switch (event) {
        default:
            break;
    }

    if (!self->complete && player_is_finished(self->player) &&
        opponents_are_finished(self->opponents)) {
        self->complete = true;
        SDL_Log("Stage complete");
    }
}

bool stage_is_complete(Stage* self) {
    return self->complete;
}

void stage_destroy(Stage* self) {
    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_destroy(self->opponents[i]);
    }
    free(self);
}

static bool opponents_are_finished(Opponent* const* opponents) {
    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        if (!opponents[i]->finished) {
            return false;
        }
    }
    return true;
}
