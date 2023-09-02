#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "events.h"
#include "indicator.h"

#define MAX_PLAYER_NAME_CHARS 100
#define MILK_CAPACITY 4000
#define MILK_SIP 100

#define PLAYER_SPRITE_WIDTH 16
#define PLAYER_SPRITE_HEIGHT 16
#define PLAYER_SPRITE_IDLE_FULL 0
#define PLAYER_SPRITE_DRINK_START 1
#define PLAYER_SPRITE_DRINK_NUM 8
#define PLAYER_SPRITE_IDLE_EMPTY 10

typedef struct {
    char name[MAX_PLAYER_NAME_CHARS];
    bool finished;
    uint16_t milk_consumed;
    uint64_t start_time;
    uint64_t drink_duration;
    SDL_Texture* sprite_sheet;
    uint8_t sprite;
    int64_t x;
    int64_t y;
    Indicator* indicator;
} Player;

Player* player_create(
    SDL_Renderer* renderer, TextureManager* texture_manager,
    const char* name, const char* texture_id, int64_t x, int64_t y
);
void player_update(Player* self, Event event);
void player_render(Player* self, SDL_Renderer* renderer, SDL_Window* window);
void player_destroy(Player* self);
