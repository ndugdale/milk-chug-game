#include "stage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>

#include "events.h"
#include "font.h"
#include "game.h"
#include "opponent.h"
#include "player.h"
#include "render.h"
#include "texture.h"

static bool opponents_are_finished(Opponent* const* opponents);

Stage* stage_create(
    SDL_Renderer* renderer, FontManager* font_manager,
    TextureManager* texture_manager, Player* player
) {
    Stage* self = (Stage*)malloc(sizeof(Stage));

    self->background = texture_manager_get(texture_manager, "stage0");
    self->player = player;
    self->min_drink_duration = 12'000;
    self->max_drink_duration = 16'000;

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        const uint64_t range = self->max_drink_duration -
            self->min_drink_duration;

        const uint64_t drink_duration = self->min_drink_duration +
            (rand() % range);

        const int64_t shift_x = (i - 1) - ((i <= 1) ? 1 : 0);
        const int64_t opp_x = (BACKGROUND_WIDTH - PLAYER_SPRITE_WIDTH) / 2 +
            PLAYER_SPRITE_WIDTH * shift_x;
        const int64_t opp_y = (BACKGROUND_HEIGHT + PLAYER_SPRITE_HEIGHT) / 2;

        self->opponents[i] = opponent_create(
            renderer, texture_manager, drink_duration, i, opp_x, opp_y
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
