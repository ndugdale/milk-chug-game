#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "font.h"
#include "opponent.h"
#include "player.h"

#define NUM_PARTICIPANTS 5
#define MAX_SCOREBOARD_PLACE_CHARS 2
#define MAX_SCOREBOARD_NAME_CHARS 40
#define MAX_SCOREBOARD_DURATION_CHARS 6

#define SCOREBOARD_SPRITE_WIDTH 156
#define SCOREBOARD_SPRITE_HEIGHT 74

#define SCOREBOARD_TEXT_PLACE_X1 2
#define SCOREBOARD_TEXT_PLACE_X2 13
#define SCOREBOARD_TEXT_NAME_X1 16
#define SCOREBOARD_TEXT_NAME_X2 126
#define SCOREBOARD_TEXT_DURATION_X1 132
#define SCOREBOARD_TEXT_DURATION_X2 151
#define SCOREBOARD_TEXT_OFFSET_Y 12

#define SCOREBOARD_LINE_TEXT_WIDTH 30

typedef struct {
    bool show;
    FontManager* font_manager;
    SDL_Texture* texture;
    char names[NUM_PARTICIPANTS][MAX_SCOREBOARD_NAME_CHARS];
    char durations[NUM_PARTICIPANTS][MAX_SCOREBOARD_DURATION_CHARS];
    bool text_set;
    int64_t x;
    int64_t y;
} Scoreboard;

Scoreboard* scoreboard_create(FontManager* font_manager, TextureManager* texture_manager, int64_t x, int64_t y);
void scoreboard_update(Scoreboard* self, Event event, Player* player, Opponent* const* opponents);
void scoreboard_set_show(Scoreboard* self, bool show);
void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window);
void scoreboard_destroy(Scoreboard* self);
