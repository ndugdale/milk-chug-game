#include "scene.h"

#include <SDL2/SDL.h>

#include "events.h"
#include "font.h"
#include "opponent.h"
#include "player.h"
#include "texture.h"

Scene* scene_create(FontManager* font_manager, TextureManager* texture_manager, Player* player) {
    Scene* self = malloc(sizeof(Scene));
    self->stage_id = 0;
    self->type = STAGE_TYPE;
    self->value.stage = stage_create_from_id(
        font_manager, texture_manager,
        player, self->stage_id
    );

    self->font_manager = font_manager;
    self->texture_manager = texture_manager;
    self->player = player;

    self->is_complete = false;

    return self;
}

void scene_update(Scene* self, Event event) {
    switch (self->type) {
        case STAGE_TYPE:
            stage_update(self->value.stage, event);
            if (self->value.stage->is_complete) {
                self->is_complete = true;
            }
            break;
        case SCOREBOARD_TYPE:
            scoreboard_update(self->value.scoreboard, event);
            if (self->value.scoreboard->is_complete) {
                self->is_complete = true;
            }
            break;
        default:
            break;
    }

    if (self->is_complete) {
        scene_next(self);
        self->is_complete = false;
    }
}

void scene_render(Scene* self, SDL_Renderer* renderer, SDL_Window* window) {
    switch (self->type) {
        case STAGE_TYPE:
            stage_render(self->value.stage, renderer, window);
            break;
        case SCOREBOARD_TYPE:
            scoreboard_render(self->value.scoreboard, renderer, window);
            break;
        default:
            break;
    }
}
void scene_next(Scene* self) {
    switch (self->type) {
        case STAGE_TYPE:
            Opponent** opponents = self->value.stage->opponents;
            Scoreboard* scoreboard = scoreboard_create(self->font_manager, self->player, opponents);
            self->type = SCOREBOARD_TYPE;
            self->value.scoreboard = scoreboard;
            break;
        case SCOREBOARD_TYPE:
            player_reset(self->player);
            self->stage_id++;
            Stage* stage = stage_create_from_id(
                self->font_manager, self->texture_manager,
                self->player, self->stage_id
            );
            self->type = STAGE_TYPE;
            self->value.stage = stage;
        default:
            break;
    }
}
void scene_destroy(Scene* self) {
    switch (self->type) {
        case STAGE_TYPE:
            stage_destroy(self->value.stage);
            break;
        case SCOREBOARD_TYPE:
            scoreboard_destroy(self->value.scoreboard);
            break;
        default:
            break;
    }
    free(self);
}
