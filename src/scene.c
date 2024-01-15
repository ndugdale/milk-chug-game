#include "scene.h"

#include <SDL.h>

#include "events.h"
#include "font.h"
#include "game.h"
#include "opponent.h"
#include "player.h"
#include "stage.h"
#include "texture.h"
#include "ui.h"

static TextDisplay* menu_create(FontManager* font_manager, TextureManager* texture_manager);
static TextDisplay* tutorial_create(FontManager* font_manager);
static TextDisplay* win_screen_create(FontManager* font_manager);
static TextDisplay* lose_screen_create(FontManager* font_manager);

static void quit_icon_next(Scene* self);
static void music_icon_next(Scene* self);
static void music_icon_toggle(Scene* self);

Scene* scene_create(
    AudioManager* audio_manager, FontManager* font_manager,
    TextureManager* texture_manager, Player* player
) {
    Scene* self = malloc(sizeof(Scene));
    self->stage_id = 0;
    self->type = MENU_TYPE;
    self->value.text_display = menu_create(font_manager, texture_manager);
    self->stage_buffer = NULL;

    self->audio_manager = audio_manager;
    self->font_manager = font_manager;
    self->texture_manager = texture_manager;
    self->player = player;

    self->quit_icon = sprite_display_create(
        self->texture_manager, "quit", WINDOW_COORDINATES,
        ICON_WIDTH, ICON_HEIGHT, ICON_WIDTH / 2, ICON_HEIGHT / 2
    );
    self->music_icon = sprite_display_create(
        self->texture_manager, "music", WINDOW_COORDINATES,
        ICON_WIDTH, ICON_HEIGHT, 3 * ICON_WIDTH / 2, ICON_HEIGHT / 2
    );
    self->is_complete = false;

    return self;
}

void scene_update(Scene* self, Event event) {
    switch (self->type) {
        case MENU_TYPE:
        case TUTORIAL_TYPE:
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
            text_display_update(self->value.text_display, event);
            if (self->value.text_display->is_complete) {
                self->is_complete = true;
            }
            break;
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

    switch (event) {
        case EVENT_TOGGLE_AUDIO:
            music_icon_toggle(self);
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
        case MENU_TYPE:
        case TUTORIAL_TYPE:
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
            text_display_render(self->value.text_display, renderer, window);
            break;
        case STAGE_TYPE:
            stage_render(self->value.stage, renderer, window);
            break;
        case SCOREBOARD_TYPE:
            scoreboard_render(self->value.scoreboard, renderer, window);
            break;
        default:
            break;
    }
    sprite_display_render(self->quit_icon, renderer, window);
    sprite_display_render(self->music_icon, renderer, window);
}

void scene_next(Scene* self) {
    TextDisplay* tutorial = NULL;
    Stage* stage_0 = NULL;
    Opponent** opponents = NULL;
    Stage* stage = NULL;
    TextDisplay* win_screen = NULL;
    TextDisplay* lose_screen = NULL;


    switch (self->type) {
        case MENU_TYPE:
            tutorial = tutorial_create(self->font_manager);
            text_display_destroy(self->value.text_display);
            self->type = TUTORIAL_TYPE;
            self->value.text_display = tutorial;
            break;
        case TUTORIAL_TYPE:
            stage_0 = stage_create_from_id(
                self->audio_manager, self->font_manager,
                self->texture_manager, self->player, 0
            );
            text_display_destroy(self->value.text_display);
            self->type = STAGE_TYPE;
            self->value.stage = stage_0;
            break;
        case STAGE_TYPE:
            opponents = self->value.stage->opponents;
            Scoreboard* scoreboard = scoreboard_create(self->font_manager, self->player, opponents);
            self->stage_buffer = self->value.stage;
            self->type = SCOREBOARD_TYPE;
            self->value.scoreboard = scoreboard;
            break;
        case SCOREBOARD_TYPE:
            player_reset(self->player);
            const bool stage_remains = self->stage_id < NUM_STAGES - 1;
            const bool stage_won = self->value.scoreboard->is_player_winner;
            if (stage_remains && stage_won) {
                self->stage_id++;
                stage = stage_create_from_id(
                    self->audio_manager, self->font_manager, self->texture_manager,
                    self->player, self->stage_id
                );
                scoreboard_destroy(self->value.scoreboard);
                self->type = STAGE_TYPE;
                self->value.stage = stage;
            } else if (stage_won) {
                win_screen = win_screen_create(self->font_manager);
                scoreboard_destroy(self->value.scoreboard);
                self->type = WIN_SCREEN_TYPE;
                self->value.text_display = win_screen;
            } else {
                lose_screen = lose_screen_create(self->font_manager);
                scoreboard_destroy(self->value.scoreboard);
                self->type = LOSE_SCREEN_TYPE;
                self->value.text_display = lose_screen;
            }
            stage_destroy(self->stage_buffer);
            self->stage_buffer = NULL;
            break;
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
            self->stage_id = 0;
            self->type = MENU_TYPE;
            self->value.text_display = menu_create(self->font_manager, self->texture_manager);
        default:
            break;
    }
    quit_icon_next(self);
    music_icon_next(self);
}

void scene_destroy(Scene* self) {
    switch (self->type) {
        case MENU_TYPE:
        case TUTORIAL_TYPE:
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
            text_display_destroy(self->value.text_display);
            break;
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

static TextDisplay* menu_create(FontManager* font_manager, TextureManager* texture_manager) {
    const int64_t title_x = (BACKGROUND_WIDTH - TITLE_SPRITE_WIDTH) / 2;
    const int64_t title_y = (BACKGROUND_HEIGHT - 3 * TITLE_SPRITE_HEIGHT / 2) / 2;

    SpriteDisplay* title_display = sprite_display_create(
        texture_manager, "title", LOCAL_COORDINATES,
        TITLE_SPRITE_WIDTH, TITLE_SPRITE_HEIGHT, title_x, title_y
    );

    return text_display_create(font_manager, title_display, "", "Press x to start");
}

static TextDisplay* tutorial_create(FontManager* font_manager) {
    return text_display_create(
        font_manager, NULL, "Press x quickly to drink", "Press x to continue"
    );
}

static TextDisplay* win_screen_create(FontManager* font_manager) {
    return text_display_create(
        font_manager, NULL, "You Win!", "Press x to return to menu"
    );
}

static TextDisplay* lose_screen_create(FontManager* font_manager) {
    return text_display_create(
        font_manager, NULL, "You Lose!", "Press x to return to menu"
    );
}

static void quit_icon_next(Scene* self) {
    switch (self->type) {
        case MENU_TYPE:
        case TUTORIAL_TYPE:
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
        case SCOREBOARD_TYPE:
            self->quit_icon->sprite = QUIT_ICON_LIGHT;
            break;
        case STAGE_TYPE:
            if (self->stage_id == NUM_STAGES - 1) {
                self->quit_icon->sprite = QUIT_ICON_LIGHT;
            } else {
                self->quit_icon->sprite = QUIT_ICON_DARK;
            }
            break;
        default:
            break;
    }
}

static void music_icon_next(Scene* self) {
    bool muted = self->audio_manager->muted;

    switch (self->type) {
        case MENU_TYPE:
        case TUTORIAL_TYPE:
        case WIN_SCREEN_TYPE:
        case LOSE_SCREEN_TYPE:
        case SCOREBOARD_TYPE:
            self->music_icon->sprite = muted ? MUSIC_ICON_MUTED_LIGHT : MUSIC_ICON_UNMUTED_LIGHT;
            break;
        case STAGE_TYPE:
            if (self->stage_id == NUM_STAGES - 1) {
                self->music_icon->sprite = muted ? MUSIC_ICON_MUTED_LIGHT : MUSIC_ICON_UNMUTED_LIGHT;
            } else {
                self->music_icon->sprite = muted ? MUSIC_ICON_MUTED_DARK : MUSIC_ICON_UNMUTED_DARK;
            }
            break;
        default:
            break;
    }
}

static void music_icon_toggle(Scene* self) {
    switch (self->music_icon->sprite) {
        case MUSIC_ICON_UNMUTED_LIGHT:
            self->music_icon->sprite = MUSIC_ICON_MUTED_LIGHT;
            break;
        case MUSIC_ICON_MUTED_LIGHT:
            self->music_icon->sprite = MUSIC_ICON_UNMUTED_LIGHT;
            break;
        case MUSIC_ICON_UNMUTED_DARK:
            self->music_icon->sprite = MUSIC_ICON_MUTED_DARK;
            break;
        case MUSIC_ICON_MUTED_DARK:
            self->music_icon->sprite = MUSIC_ICON_UNMUTED_DARK;
            break;
        default:
            break;
    }
}
