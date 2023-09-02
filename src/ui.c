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

static void update_text(Scoreboard* self, Player* player, Opponent* const* opponents);

Scoreboard* scoreboard_create(FontManager* font_manager, TextureManager* texture_manager, int64_t x, int64_t y) {
    Scoreboard* self = (calloc(1, sizeof(Scoreboard)));
    self->show = false;
    self->font_manager = font_manager;
    self->texture = texture_manager_get(texture_manager, "scoreboard");
    self->text_set = false;
    self->x = x;
    self->y = y;
}

void scoreboard_update(Scoreboard* self, Event event, Player* player, Opponent* const* opponents) {
    switch (event) {
        case EVENT_TICK:
            update_text(self, player, opponents);
        default:
            break;
    }
}

void scoreboard_show(Scoreboard* self) {
    self->show = true;
}

void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window) {
    if (self->show) {
        int64_t window_x;
        int64_t window_y;
        local_xy_to_window_xy(
            window, self->x, self->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
            &window_x, &window_y
        );

        TTF_Font* font = font_manager_get(self->font_manager, "munro_10");
        int32_t font_height = TTF_FontLineSkip(font);
        SDL_Color colour = {255, 212, 26, 255};

        blit_sprite(
            renderer, self->texture, 0, 0, window_x, window_y,
            SCOREBOARD_SPRITE_WIDTH, SCOREBOARD_SPRITE_HEIGHT
        );

        for (size_t i; i < NUM_PARTICIPANTS; i++) {
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
}

void scoreboard_destroy(Scoreboard* self) {
    free(self);
}

static void update_text(Scoreboard* self, Player* player, Opponent* const* opponents) {
    if (self->show && !self->text_set) {
        uint64_t durations[NUM_PARTICIPANTS];
        self->text_set = true;

        // Retrieve list of all drink durations
        durations[0] = player->drink_duration;
        for (size_t i = 0; i < NUM_OPPONENTS; i++) {
            durations[i + 1] = opponents[i]->drink_duration;
        }

        // Sort drink durations
        uint64_sort(durations, NUM_PARTICIPANTS);

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
    }
}
