#include "opponent.h"

#include <stdbool.h>
#include <string.h>

#include "events.h"
#include "player.h"
#include "render.h"
#include "texture.h"

static void opponent_drink(Opponent* self);

Opponent* opponent_create(
    TextureManager* texture_manager, const char* name, const char* texture_id,
    uint64_t drink_duration, uint64_t x, uint64_t y
) {
    Opponent* self = malloc(sizeof(Opponent));
    strcpy(self->name, name);
    self->start_time = SDL_GetTicks64();
    self->drink_duration = drink_duration;
    self->finished = false;
    self->milk_consumed = 0;
    self->sprite = PLAYER_SPRITE_IDLE_FULL;
    self->x = x;
    self->y = y;
    self->indicator = indicator_create(
        texture_manager, x,
        y - INDICATOR_VERTICAL_OFFSET - INDICATOR_SPRITE_HEIGHT
    );

    self->sprite_sheet = texture_manager_get(texture_manager, texture_id);

    return self;
}

void opponent_update(Opponent* self, Event event) {
    indicator_update(self->indicator, event, self->milk_consumed, MILK_CAPACITY);

    switch (event) {
        case EVENT_TICK:
            opponent_drink(self);
        default:
            break;
    }
}

void opponent_render(
    Opponent* self, SDL_Renderer* renderer, SDL_Window* window
) {
    int64_t window_x;
    int64_t window_y;
    local_xy_to_window_xy(
        renderer, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
        &window_x, &window_y
    );

    blit_sprite(
        renderer, self->sprite_sheet, 0, self->sprite, window_x, window_y,
        PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT
    );

    indicator_render(self->indicator, renderer, window);
}

void opponent_reset(Opponent* self) {
    self->finished = false;
    self->milk_consumed = 0;
    self->sprite = PLAYER_SPRITE_IDLE_FULL;
    self->start_time = SDL_GetTicks64();
}

void opponent_destroy(Opponent* self) {
    indicator_destroy(self->indicator);
    free(self);
}

static void opponent_drink(Opponent* self) {
    if (!self->finished) {
        const uint64_t current_time = SDL_GetTicks64();
        const uint64_t elapsed_time = current_time - self->start_time;

        self->milk_consumed = MILK_CAPACITY *
            elapsed_time /
            self->drink_duration;

        self->sprite = PLAYER_SPRITE_DRINK_START +
            self->milk_consumed * PLAYER_SPRITE_DRINK_NUM / MILK_CAPACITY;

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
        }
    }
}
