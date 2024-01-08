#include "ui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "events.h"
#include "font.h"
#include "opponent.h"
#include "player.h"
#include "render.h"
#include "stage.h"
#include "utils.h"

SpriteDisplay* sprite_display_create(
    TextureManager* texture_manager, const char* texture_id,
    CoordinateSystem coordinate_system, uint32_t w, uint32_t h, int64_t x, int64_t y
) {
    SpriteDisplay* self = (calloc(1, sizeof(SpriteDisplay)));
    self->sprite_sheet = texture_manager_get(texture_manager, texture_id);
    self->sprite = 0;
    self->coordinate_system = coordinate_system;
    self->x = x;
    self->y = y;
    self->w = w;
    self->h = h;

    return self;
}

void sprite_display_render(SpriteDisplay* self, SDL_Renderer* renderer, SDL_Window* window) {
    int64_t x;
    int64_t y;

    if (self->coordinate_system == LOCAL_COORDINATES) {
        local_xy_to_window_xy(window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, &x, &y);
    } else {
        x = self->x;
        y = self->y;
    }

    blit_sprite(
        renderer, self->sprite_sheet, 0, self->sprite, x, y,
        self->w, self->h
    );
}

void sprite_display_destroy(SpriteDisplay* self) {
    free(self);
}

TextDisplay* text_display_create(
    FontManager* font_manager, SpriteDisplay* title_sprite,
    const char* primary_text, const char* secondary_text
) {
    TextDisplay* self = (calloc(1, sizeof(TextDisplay)));
    self->font_manager = font_manager;
    self->title_sprite = title_sprite;
    self->primary_text = primary_text;
    self->secondary_text = secondary_text;
    self->is_complete = false;

    return self;
}

void text_display_update(TextDisplay* self, Event event) {
    switch (event) {
        case EVENT_ACTION:
            self->is_complete = true;
            break;
        default:
            break;
    }
}

void text_display_render(TextDisplay* self, SDL_Renderer* renderer, SDL_Window* window) {
    TTF_Font* primary_font = font_manager_get(self->font_manager, "munro_20");
    TTF_Font* secondary_font = font_manager_get(self->font_manager, "munro_10");
    int32_t primary_font_height = TTF_FontLineSkip(primary_font);
    int32_t secondary_font_height = TTF_FontLineSkip(secondary_font);
    SDL_Color colour = {255, 212, 26, 255};

    uint32_t window_height;
    uint32_t window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);
    const int64_t scaled_window_width = (double)window_width / (double)RENDERER_SCALE_FACTOR;
    const int64_t scaled_window_height = (double)window_height / (double)RENDERER_SCALE_FACTOR;
    const int64_t primary_y = scaled_window_height / 2 - primary_font_height;
    const int64_t secondary_y = scaled_window_height / 2 + 2 * secondary_font_height;

    blit_text(
        renderer, primary_font, self->primary_text, colour,
        0, scaled_window_width, primary_y, CentreAligned
    );

    blit_text(
        renderer, secondary_font, self->secondary_text, colour,
        0, scaled_window_width, secondary_y, CentreAligned
    );

    if (self->title_sprite != NULL) {
        sprite_display_render(self->title_sprite, renderer, window);
    }
}

void text_display_destroy(TextDisplay* self) {
    free(self);
}

Countdown* countdown_create(AudioManager* audio_manager, TextureManager* texture_manager, int64_t x, int64_t y) {
    Countdown* self = (calloc(1, sizeof(Countdown)));
    self->audio_manager = audio_manager;
    self->start_time = SDL_GetTicks64();
    self->sprite_sheet = texture_manager_get(texture_manager, "countdown");
    self->sprite = COUNTDOWN_NONE;
    self->x = x;
    self->y = y;

    return self;
}

void countdown_update(Countdown* self, Event event) {
    switch (event) {
        case EVENT_TICK:
            if (
                self->sprite != COUNTDOWN_THREE &&
                SDL_GetTicks64() > self->start_time + PRE_COUNTDOWN_MS &&
                SDL_GetTicks64() < self->start_time + PRE_COUNTDOWN_MS + 1000
            ) {
                self->sprite = COUNTDOWN_THREE;
                audio_manager_play_effect(self->audio_manager, "beep_low");
            } else if (
                self->sprite != COUNTDOWN_TWO &&
                SDL_GetTicks64() > self->start_time + PRE_COUNTDOWN_MS + 1000 &&
                SDL_GetTicks64() < self->start_time + PRE_COUNTDOWN_MS + 2000
            ) {
                self->sprite = COUNTDOWN_TWO;
                audio_manager_play_effect(self->audio_manager, "beep_low");
            } else if (
                self->sprite != COUNTDOWN_ONE &&
                SDL_GetTicks64() > self->start_time + PRE_COUNTDOWN_MS + 2000 &&
                SDL_GetTicks64() < self->start_time + PRE_COUNTDOWN_MS + 3000
            ) {
                self->sprite = COUNTDOWN_ONE;
                audio_manager_play_effect(self->audio_manager, "beep_low");
            } else if (
                self->sprite != COUNTDOWN_GO &&
                SDL_GetTicks64() > self->start_time + PRE_COUNTDOWN_MS + 3000 &&
                SDL_GetTicks64() < self->start_time + PRE_COUNTDOWN_MS + 3000 + POST_COUNTDOWN_MS
            ) {
                self->sprite = COUNTDOWN_GO;
                audio_manager_play_effect(self->audio_manager, "beep_high");
            } else if (
                self->sprite != COUNTDOWN_NONE &&
                SDL_GetTicks64() > self->start_time + PRE_COUNTDOWN_MS + 3000 + POST_COUNTDOWN_MS
            ) {
                self->sprite = COUNTDOWN_NONE;
            }
            break;
        default:
            break;
    }
}

void countdown_render(Countdown* self, SDL_Renderer* renderer, SDL_Window* window) {
    int64_t window_x;
    int64_t window_y;

    if (self->sprite != COUNTDOWN_NONE) {
        local_xy_to_window_xy(
            window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
            &window_x, &window_y
        );

        blit_sprite(
            renderer, self->sprite_sheet, 0, self->sprite, window_x, window_y,
            COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT
        );
    }
}

void countdown_destroy(Countdown* self) {
    free(self);
}

Scoreboard* scoreboard_create(FontManager* font_manager, Player* player, Opponent* const* opponents) {
    Scoreboard* self = (calloc(1, sizeof(Scoreboard)));
    self->font_manager = font_manager;
    self->x = (BACKGROUND_WIDTH - SCOREBOARD_WIDTH) / 2;
    self->y = (BACKGROUND_HEIGHT - SCOREBOARD_HEIGHT) / 2;
    self->is_complete = false;
    self->is_player_winner = false;
    uint64_t durations[NUM_PARTICIPANTS];

    // Retrieve list of all drink durations
    durations[0] = player->drink_duration;
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        durations[i + 1] = opponents[i]->drink_duration;
    }

    // Sort drink durations
    uint64_sort(durations, NUM_PARTICIPANTS);
    if (durations[0] == player->drink_duration) {
        self->is_player_winner = true;
    }

    // Build scoreboard string from sorted durations
    for (size_t i = 0; i < NUM_PARTICIPANTS; i++) {
        char buffer[MAX_SCOREBOARD_DURATION_CHARS];

        if (player->drink_duration == durations[i]) {
            snprintf(
                buffer, MAX_SCOREBOARD_DURATION_CHARS, "%.2lf",
                (double)player->drink_duration / 1000.0
            );
            strcat(self->names[i], player->name);
            strcat(self->durations[i], buffer);
        } else {
            for (size_t j = 0; j < NUM_OPPONENTS; j++) {
                if (opponents[j]->drink_duration == durations[i]) {
                    snprintf(
                        buffer, MAX_SCOREBOARD_DURATION_CHARS, "%.2lf",
                        (double)opponents[j]->drink_duration / 1000.0
                    );
                    strcat(self->names[i], opponents[j]->name);
                    strcat(self->durations[i], buffer);
                }
            }
        }
    }

    return self;
}

void scoreboard_update(Scoreboard* self, Event event) {
    switch (event) {
        case EVENT_ACTION:
            self->is_complete = true;
            break;
        default:
            break;
    }
}

void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window) {
    int64_t window_x;
    int64_t window_y;
    local_xy_to_window_xy(
        window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
        &window_x, &window_y
    );

    TTF_Font* font = font_manager_get(self->font_manager, "munro_10");
    int32_t font_height = TTF_FontLineSkip(font);
    SDL_Color colour = {255, 212, 26, 255};

    blit_background_colour(renderer, 0, 0, 0, 255);

    for (size_t i = 0; i < NUM_PARTICIPANTS; i++) {
        char buffer[MAX_SCOREBOARD_PLACE_CHARS];
        snprintf(buffer, MAX_SCOREBOARD_PLACE_CHARS, "%d", i + 1);

        blit_text(
            renderer, font, buffer, colour, window_x + SCOREBOARD_TEXT_PLACE_X1,
            window_x + SCOREBOARD_TEXT_PLACE_X2,
            window_y + SCOREBOARD_TEXT_OFFSET_Y + i * font_height,
            CentreAligned
        );

        blit_text(
            renderer, font, self->names[i], colour, window_x + SCOREBOARD_TEXT_NAME_X1,
            window_x + SCOREBOARD_TEXT_NAME_X2,
            window_y + SCOREBOARD_TEXT_OFFSET_Y + i * font_height,
            LeftAligned
        );

        blit_text(
            renderer, font, self->durations[i], colour, window_x + SCOREBOARD_TEXT_DURATION_X1,
            window_x + SCOREBOARD_TEXT_DURATION_X2,
            window_y + SCOREBOARD_TEXT_OFFSET_Y + i * font_height,
            RightAligned
        );
    }
}

void scoreboard_destroy(Scoreboard* self) {
    free(self);
}
