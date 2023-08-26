#include "indicator.h"

#include "game.h"
#include "render.h"
#include "texture.h"

static void indicator_tick(Indicator* self, uint16_t capacity, uint16_t max);

Indicator* indicator_create(SDL_Renderer* renderer, TextureManager* texture_manager, int64_t x, int64_t y) {
    Indicator* self = (Indicator*)malloc(sizeof(Indicator));
    self->sprite_sheet = texture_manager_get(texture_manager, "indicator");
    self->x = x;
    self->y = y;

    return self;
}

void indicator_update(Indicator* self, Event event, uint16_t capacity, uint16_t max) {
    switch (event) {
        case EVENT_TICK:
            indicator_tick(self, capacity, max);
        default:
            break;
    }
}

void indicator_render(Indicator* self, SDL_Renderer* renderer, SDL_Window* window) {
    int64_t window_x;
    int64_t window_y;
    local_xy_to_window_xy(
        window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
        &window_x, &window_y
    );

    blit_sprite(
        renderer, self->sprite_sheet, 0, self->sprite, window_x, window_y,
        INDICATOR_SPRITE_WIDTH, INDICATOR_SPRITE_HEIGHT
    );
}

void indicator_destroy(Indicator* self) {
    free(self);
}

static void indicator_tick(Indicator* self, uint16_t capacity, uint16_t max) {
    if (capacity < max) {
        self->sprite = capacity * INDICATOR_SPRITE_NUM / max;
    } else {
        self->sprite = INDICATOR_SPRITE_NUM - 1;
    }
}
