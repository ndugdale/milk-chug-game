#include "player.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "events.h"
#include "render.h"

static void player_drink(Player* self);

Player* player_create(
    TextureManager* texture_manager, const char* name,
    const char* texture_id, int64_t x, int64_t y
) {
    Player* self = malloc(sizeof(Player));
    strcpy(self->name, name);
    self->sprite_sheet = texture_manager_get(texture_manager, texture_id);
    self->x = x;
    self->y = y;
    self->indicator = indicator_create(
        texture_manager, x,
        y - INDICATOR_VERTICAL_OFFSET - INDICATOR_SPRITE_HEIGHT
    );

    player_reset(self);

    return self;
}

void player_update(Player* self, Event event) {
    indicator_update(self->indicator, event, self->milk_consumed, MILK_CAPACITY);

    switch (event) {
        case EVENT_ACTION:
            player_drink(self);
        default:
            break;
    }
}

void player_render(Player* self, SDL_Renderer* renderer, SDL_Window* window) {
    int64_t window_x;
    int64_t window_y;
    local_xy_to_window_xy(
        window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
        &window_x, &window_y
    );

    blit_sprite(
        renderer, self->sprite_sheet, 0, self->sprite, window_x, window_y,
        PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT
    );

    indicator_render(self->indicator, renderer, window);
}

void player_destroy(Player* self) {
    indicator_destroy(self->indicator);
    free(self);
}

void player_reset(Player* self) {
    self->finished = false;
    self->milk_consumed = 0;
    self->drink_duration = UINT64_MAX;
    self->sprite = PLAYER_SPRITE_IDLE_FULL;
    self->start_time = SDL_GetTicks64();
}

static void player_drink(Player* self) {
    if (!self->finished) {
        self->milk_consumed += MILK_SIP;
        SDL_Log("GLUG: %d/%d", self->milk_consumed, MILK_CAPACITY);

        self->sprite = PLAYER_SPRITE_DRINK_START +
            self->milk_consumed * PLAYER_SPRITE_DRINK_NUM / MILK_CAPACITY;

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->drink_duration = SDL_GetTicks64() - self->start_time;
            self->finished = true;
            SDL_Log("You finished in %d ms", self->drink_duration);
        }
    }
}
