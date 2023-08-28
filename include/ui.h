#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "font.h"
#include "opponent.h"
#include "player.h"

#define NUM_PARTICIPANTS 5
#define MAX_SCOREBOARD_LINE_CHARS 600

typedef struct {
    bool show;
    FontManager* font_manager;
    char text[MAX_SCOREBOARD_LINE_CHARS];
    bool text_set;
} Scoreboard;

Scoreboard* scoreboard_create(FontManager* font_manager);
void scoreboard_update(Scoreboard* self, Event event, Player* player, Opponent* const* opponents);
void scoreboard_set_show(Scoreboard* self, bool show);
void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window);
void scoreboard_destroy(Scoreboard* self);
