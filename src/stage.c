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
    Stage* self = (Stage*)malloc(sizeof(Stage));

    self->background = load_texture(renderer, "assets/images/purple_sky.png");
    self->player = player;
    self->min_drink_duration = 12'000;
    self->max_drink_duration = 16'000;

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        uint64_t range = self->max_drink_duration - self->min_drink_duration;
        uint64_t drink_duration = self->min_drink_duration + (rand() % range);
        self->opponents[i] = opponent_create(
            renderer, drink_duration, i,
            PLAYER_SPRITE_WIDTH * (i + 1), 0
        );
    }

    return self;
}

void stage_update(Stage* self, Event event) {
    player_update(self->player, event);

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
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
    blit_background(
        renderer, window, self->background, (double)RENDERER_SCALE_FACTOR,
        BACKGROUND_WIDTH, BACKGROUND_HEIGHT
    );

    player_render(self->player, renderer, window);

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_render(self->opponents[i], renderer, window);
    }
}

void stage_destroy(Stage* self) {
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_destroy(self->opponents[i]);
    }
    free(self);
}

bool stage_is_complete(const Stage* self) {
    return self->complete;
}

static bool opponents_are_finished(Opponent* const* opponents) {
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        if (!opponents[i]->finished) {
            return false;
        }
    }
    return true;
}
