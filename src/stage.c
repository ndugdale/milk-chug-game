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

static void stage_tick(Stage* self);
static bool opponents_are_finished(Opponent* const* opponents);

Stage* stage_create(
    SDL_Renderer* renderer, FontManager* font_manager,
    TextureManager* texture_manager, Player* player
) {
    Stage* self = malloc(sizeof(Stage));

    self->background = texture_manager_get(texture_manager, "stage_0");
    self->player = player;
    self->min_drink_duration = 12'000;
    self->max_drink_duration = 16'000;
    self->complete = false;

    const int64_t scoreboard_x = (BACKGROUND_WIDTH - SCOREBOARD_SPRITE_WIDTH) / 2;
    const int64_t scoreboard_y = (BACKGROUND_HEIGHT - SCOREBOARD_SPRITE_HEIGHT) / 2;
    self->scoreboard = scoreboard_create(font_manager, texture_manager, scoreboard_x, scoreboard_y);

    uint64_t drink_durations[NUM_OPPONENTS];
    int64_t x_positions[NUM_OPPONENTS];
    int64_t y_positions[NUM_OPPONENTS];
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        const uint64_t range = self->max_drink_duration - self->min_drink_duration;
        drink_durations[i] = self->min_drink_duration + (rand() % range);

        const int64_t shift_x = (i - 1) - ((i <= 1) ? 1 : 0);
        x_positions[i] = (BACKGROUND_WIDTH - PLAYER_SPRITE_WIDTH) / 2 + PLAYER_SPRITE_WIDTH * shift_x;
        y_positions[i] = (BACKGROUND_HEIGHT + PLAYER_SPRITE_HEIGHT) / 2;
    }

    self->opponents[0] = opponent_create(
        renderer, texture_manager, "Milo Dynaseur", "opponent_0_0",
        drink_durations[0], x_positions[0], y_positions[0]
    );

    self->opponents[1] = opponent_create(
        renderer, texture_manager, "Anna Mullprodukt", "opponent_0_1",
        drink_durations[1], x_positions[1], y_positions[1]
    );

    self->opponents[2] = opponent_create(
        renderer, texture_manager, "Luke Tozzintollrent", "opponent_0_2",
        drink_durations[2], x_positions[2], y_positions[2]
    );

    self->opponents[3] = opponent_create(
        renderer, texture_manager, "Hammad To'swollo", "opponent_0_3",
        drink_durations[3], x_positions[3], y_positions[3]
    );

    return self;
}

void stage_update(Stage* self, Event event) {
    player_update(self->player, event);
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_update(self->opponents[i], event);
    }
    scoreboard_update(self->scoreboard, event, self->player, self->opponents);

    switch (event) {
        case EVENT_TICK:
            stage_tick(self);
        default:
            break;
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

    scoreboard_render(self->scoreboard, renderer, window);
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

static void stage_tick(Stage* self) {
    if (!self->complete && self->player->finished && opponents_are_finished(self->opponents)) {
        scoreboard_show(self->scoreboard);
        self->complete = true;
    }
}

static bool opponents_are_finished(Opponent* const* opponents) {
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        if (!opponents[i]->finished) {
            return false;
        }
    }
    return true;
}
