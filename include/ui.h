#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "audio.h"
#include "events.h"
#include "font.h"
#include "opponent.h"
#include "player.h"

#define NUM_PARTICIPANTS 5
#define MAX_SCOREBOARD_PLACE_CHARS 2
#define MAX_SCOREBOARD_NAME_CHARS 40
#define MAX_SCOREBOARD_DURATION_CHARS 6

#define SCOREBOARD_WIDTH 156
#define SCOREBOARD_HEIGHT 74

#define SCOREBOARD_TEXT_PLACE_X1 2
#define SCOREBOARD_TEXT_PLACE_X2 13
#define SCOREBOARD_TEXT_NAME_X1 16
#define SCOREBOARD_TEXT_NAME_X2 122
#define SCOREBOARD_TEXT_DURATION_X1 128
#define SCOREBOARD_TEXT_DURATION_X2 151
#define SCOREBOARD_TEXT_OFFSET_Y 12

#define SCOREBOARD_LINE_TEXT_WIDTH 30

#define PRE_COUNTDOWN_MS 1000
#define POST_COUNTDOWN_MS 1000
#define COUNTDOWN_SPRITE_WIDTH 150
#define COUNTDOWN_SPRITE_HEIGHT 100

typedef enum {
    COUNTDOWN_THREE,
    COUNTDOWN_TWO,
    COUNTDOWN_ONE,
    COUNTDOWN_GO,
    COUNTDOWN_NONE
} CountdownSprite;

typedef struct {
    FontManager* font_manager;
    const char* primary_text;
    const char* secondary_text;
    bool is_complete;
} TextDisplay;

typedef struct {
    AudioManager* audio_manager;
    uint64_t start_time;
    SDL_Texture* sprite_sheet;
    CountdownSprite sprite;
    int64_t x;
    int64_t y;
} Countdown;

typedef struct {
    FontManager* font_manager;
    char names[NUM_PARTICIPANTS][MAX_SCOREBOARD_NAME_CHARS];
    char durations[NUM_PARTICIPANTS][MAX_SCOREBOARD_DURATION_CHARS];
    int64_t x;
    int64_t y;
    bool is_complete;
    bool is_player_winner;
} Scoreboard;

TextDisplay* text_display_create(FontManager* font_manager, const char* primary_text, const char* secondary_text);
void text_display_update(TextDisplay* self, Event event);
void text_display_render(TextDisplay* self, SDL_Renderer* renderer, SDL_Window* window);
void text_display_destroy(TextDisplay* self);

Countdown* countdown_create(AudioManager* audio_manager, TextureManager* texture_manager, int64_t x, int64_t y);
void countdown_update(Countdown* self, Event event);
void countdown_render(Countdown* self, SDL_Renderer* renderer, SDL_Window* window);
void countdown_destroy(Countdown* self);

Scoreboard* scoreboard_create(FontManager* font_manager, Player* player, Opponent* const* opponents);
void scoreboard_update(Scoreboard* self, Event event);
void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window);
void scoreboard_destroy(Scoreboard* self);
