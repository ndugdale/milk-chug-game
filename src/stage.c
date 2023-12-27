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
    TextureManager* texture_manager, Player* player,
    const char** opponent_names, const char** opponent_texture_ids,
    const char* background_texture_id,
    uint64_t min_drink_duration, uint64_t max_drink_duration
) {
    Stage* self = malloc(sizeof(Stage));

    self->background = texture_manager_get(texture_manager, background_texture_id);
    self->player = player;
    self->min_drink_duration = min_drink_duration;
    self->max_drink_duration = max_drink_duration;
    self->complete = false;

    const int64_t scoreboard_x = (BACKGROUND_WIDTH - SCOREBOARD_SPRITE_WIDTH) / 2;
    const int64_t scoreboard_y = (BACKGROUND_HEIGHT - SCOREBOARD_SPRITE_HEIGHT) / 2;
    self->scoreboard = scoreboard_create(font_manager, texture_manager, scoreboard_x, scoreboard_y);

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        const uint64_t range = self->max_drink_duration - self->min_drink_duration;
        uint64_t drink_duration = self->min_drink_duration + (rand() % range);

        const int64_t shift_x = (i - 1) - ((i <= 1) ? 1 : 0);
        int64_t x_position = (BACKGROUND_WIDTH - PLAYER_SPRITE_WIDTH) / 2 + PLAYER_SPRITE_WIDTH * shift_x;
        int64_t y_position = (BACKGROUND_HEIGHT + PLAYER_SPRITE_HEIGHT) / 2;

        self->opponents[i] = opponent_create(
            renderer, texture_manager, opponent_names[i], opponent_texture_ids[i],
            drink_duration, x_position, y_position
        );
    }

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

Stage* stage_0_create(
    SDL_Renderer* renderer, FontManager* font_manager,
    TextureManager* texture_manager, Player* player
) {
    const char* opponent_names[] = {
        "Milo Dynaseur",
        "Nessie Quick",
        "Luke Toes",
        "Hammad To'swollo"};

    const char* opponent_texture_ids[] = {
        "opponent_0_0",
        "opponent_0_1",
        "opponent_0_2",
        "opponent_0_3"};

    const char* background_texture_id = "stage_0";

    return stage_create(
        renderer, font_manager, texture_manager, player,
        opponent_names, opponent_texture_ids,
        background_texture_id, 12'000, 16'000
    );
}

Stage* stage_1_create(
    SDL_Renderer* renderer, FontManager* font_manager,
    TextureManager* texture_manager, Player* player
) {
    const char* opponent_names[] = {
        "Mama Reigland",
        "Pa Sterise",
        "Atty Fishal-Flavour",
        "Cole Oid"};

    const char* opponent_texture_ids[] = {
        "opponent_1_0",
        "opponent_1_1",
        "opponent_1_2",
        "opponent_1_3"};

    const char* background_texture_id = "stage_1";

    return stage_create(
        renderer, font_manager, texture_manager, player,
        opponent_names, opponent_texture_ids,
        background_texture_id, 10'000, 14'000
    );
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
