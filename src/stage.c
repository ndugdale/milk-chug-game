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
#include "utils.h"

static bool opponents_are_finished(Opponent* const* opponents);

Stage* stage_create(SDL_Renderer* renderer, Player* player) {
    Stage* self = (Stage*)malloc(sizeof(Stage));

    self->background = load_texture(renderer, "assets/images/purple_sky.png");
    self->player = player;
    self->min_drink_duration = 12'000;
    self->max_drink_duration = 15'000;

    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        uint64_t range = self->max_drink_duration - self->min_drink_duration;
        uint64_t drink_duration = self->min_drink_duration + (rand() % range);
        self->opponents[i] = opponent_create(drink_duration);
    }

    return self;
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

void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window) {
    uint32_t window_height;
    uint32_t window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    const uint32_t scaled_window_width =
        (double)window_width / (double)RENDERER_SCALE_FACTOR;
    const uint32_t scaled_window_height =
        (double)window_height / (double)RENDERER_SCALE_FACTOR;

    const uint32_t src_x = BACKGROUND_WIDTH >= scaled_window_width
        ? (BACKGROUND_WIDTH - scaled_window_width) / 2
        : 0;
    const uint32_t src_y = BACKGROUND_HEIGHT >= scaled_window_height
        ? (BACKGROUND_HEIGHT - scaled_window_height) / 2
        : 0;

    const uint32_t dst_x = scaled_window_width >= BACKGROUND_WIDTH
        ? (scaled_window_width - BACKGROUND_WIDTH) / 2
        : 0;
    const uint32_t dst_y = scaled_window_height >= BACKGROUND_HEIGHT
        ? (scaled_window_height - BACKGROUND_HEIGHT) / 2
        : 0;

    const SDL_Rect src_rect = {
        src_x, src_y, MIN(scaled_window_width, BACKGROUND_WIDTH),
        MIN(scaled_window_height, BACKGROUND_HEIGHT)};
    const SDL_Rect dst_rect = {
        dst_x, dst_y, MIN(scaled_window_width, BACKGROUND_WIDTH),
        MIN(scaled_window_height, BACKGROUND_HEIGHT)};

    SDL_RenderCopy(renderer, self->background, &src_rect, &dst_rect);
    player_render(self->player, renderer);
}

void stage_destroy(Stage* self) {
    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_destroy(self->opponents[i]);
    }
    free(self);
}

bool stage_is_complete(const Stage* self) {
    return self->complete;
}

static bool opponents_are_finished(Opponent* const* opponents) {
    for (uint8_t i = 0; i < NUM_OPPONENTS; i++) {
        if (!opponents[i]->finished) {
            return false;
        }
    }
    return true;
}
