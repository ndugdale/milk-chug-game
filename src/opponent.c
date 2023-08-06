#include "opponent.h"

#include <stdbool.h>

#include "events.h"
#include "player.h"
#include "render.h"

static void opponent_drink(Opponent* self);

Opponent* opponent_create(
    SDL_Renderer* renderer, uint64_t drink_duration, size_t index
) {
    Opponent* self = (Opponent*)malloc(sizeof(Opponent));
    self->start_time = 0;
    self->drink_duration = drink_duration;
    self->finished = false;
    self->milk_consumed = 0;
    self->index = index;

    char fname_buffer[MAX_FILENAME_LENGTH];
    sprintf(fname_buffer, "assets/images/opponent%d.png", self->index);
    self->sprite_sheet = load_texture(renderer, fname_buffer);

    return self;
}

void opponent_update(Opponent* self, Event event) {
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
    int64_t x;
    int64_t y;
    background_xy_to_window_xy(
        window, PLAYER_SPRITE_WIDTH * (self->index + 1), 0,
        BACKGROUND_WIDTH, BACKGROUND_HEIGHT, &x, &y
    );

    blit_sprite(
        renderer, self->sprite_sheet, 0, self->sprite, x, y,
        PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT
    );
}

void opponent_destroy(Opponent* self) {
    free(self);
}

bool opponent_is_finished(const Opponent* self) {
    return self->finished;
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
            SDL_Log("Opponent finished in %d ms", self->drink_duration);
        }
    }
}
