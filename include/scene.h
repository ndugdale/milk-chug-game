#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "events.h"
#include "font.h"
#include "player.h"
#include "stage.h"
#include "texture.h"
#include "ui.h"

typedef enum {
    STAGE_TYPE,
    SCOREBOARD_TYPE
} SceneType;

typedef union {
    Stage* stage;
    Scoreboard* scoreboard;
} SceneValue;

typedef struct {
    SceneType type;
    SceneValue value;
    uint8_t stage_id;
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
