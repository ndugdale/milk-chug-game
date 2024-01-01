#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "events.h"
#include "font.h"
#include "player.h"
#include "stage.h"
#include "texture.h"
#include "ui.h"

#define NUM_STAGES 5

typedef enum {
    MENU_TYPE,
    TUTORIAL_TYPE,
    STAGE_TYPE,
    SCOREBOARD_TYPE,
    WIN_SCREEN_TYPE,
    LOSE_SCREEN_TYPE
} SceneType;

typedef union {
    TextDisplay* text_display;
    Stage* stage;
    Scoreboard* scoreboard;
} SceneValue;

typedef struct {
    SceneType type;
    SceneValue value;
    uint8_t stage_id;
    Stage* stage_buffer;
    SDL_Renderer* renderer;
    FontManager* font_manager;
    TextureManager* texture_manager;
    Player* player;
    bool is_complete;
} Scene;

Scene* scene_create(FontManager* font_manager, TextureManager* texture_manager, Player* player);
void scene_update(Scene* self, Event event);
void scene_render(Scene* self, SDL_Renderer* renderer, SDL_Window* window);
void scene_next(Scene* self);
void scene_destroy(Scene* self);
