#include "player.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "events.h"
#include "render.h"

static void player_reset(Player* self);
static void player_drink(Player* self);

Player* player_create(SDL_Renderer* renderer, int64_t x, int64_t y) {
    Player* self = (Player*)malloc(sizeof(Player));
    self->sprite_sheet = load_texture(renderer, "assets/images/player.png");
    self->x = x;
    self->y = y;

    player_reset(self);

    return self;
}

void player_update(Player* self, Event event) {
    switch (event) {
        case EVENT_DRINK:
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
}

void player_destroy(Player* self) {
    free(self);
}

bool player_is_finished(const Player* self) {
    return self->finished;
}

static void player_reset(Player* self) {
    self->finished = false;
    self->milk_consumed = 0;
    self->drink_duration = UINT64_MAX;
    self->sprite = PLAYER_SPRITE_IDLE_FULL;
}

static void player_drink(Player* self) {
    if (!self->finished) {
        self->milk_consumed += MILK_SIP;
        SDL_Log("GLUG: %d/%d", self->milk_consumed, MILK_CAPACITY);

        self->sprite = PLAYER_SPRITE_DRINK_START +
            self->milk_consumed * PLAYER_SPRITE_DRINK_NUM / MILK_CAPACITY;

        if (self->milk_consumed >= MILK_CAPACITY) {
            self->finished = true;
            SDL_Log("You finished");
        }
    }
}
