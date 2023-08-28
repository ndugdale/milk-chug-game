#include "ui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "events.h"
#include "font.h"
#include "opponent.h"
#include "player.h"
#include "stage.h"
#include "utils.h"

static void scoreboard_update_text(Scoreboard* self, Player* player, Opponent* const* opponents);

Scoreboard* scoreboard_create(FontManager* font_manager) {
    Scoreboard* self = (calloc(1, sizeof(Scoreboard)));
    self->show = false;
    self->font_manager = font_manager;
    self->text_set = false;
}

void scoreboard_update(Scoreboard* self, Event event, Player* player, Opponent* const* opponents) {
    switch (event) {
        case EVENT_TICK:
            scoreboard_update_text(self, player, opponents);
        default:
            break;
    }
}

void scoreboard_set_show(Scoreboard* self, bool show) {
    self->show = show;
}

void scoreboard_render(Scoreboard* self, SDL_Renderer* renderer, SDL_Window* window) {
    if (self->show) {
        TTF_Font* font = font_manager_get(self->font_manager, "munro_10");
        int32_t font_height = TTF_FontLineSkip(font);
        SDL_Color colour = {255, 0, 0, 255};

        blit_text(renderer, font, self->text, colour, 10, 0);
    }
}

void scoreboard_destroy(Scoreboard* self) {
    free(self);
}

static void scoreboard_update_text(Scoreboard* self, Player* player, Opponent* const* opponents) {
    if (self->show && !self->text_set) {
        uint64_t durations[NUM_PARTICIPANTS];
        char buffer[MAX_SCOREBOARD_LINE_CHARS];

        self->text_set = true;

        // Retrieve list of all drink durations
        durations[0] = player_get_drink_duration(player);
        for (size_t i = 0; i < NUM_OPPONENTS; i++) {
            durations[i + 1] = opponent_get_drink_duration(opponents[i]);
        }

        // Sort drink durations
        uint64_sort(durations, NUM_PARTICIPANTS);

        // Build scoreboard string from sorted durations
        for (size_t i = 0; i < NUM_PARTICIPANTS; i++) {
            if (player->drink_duration == durations[i]) {
                snprintf(
                    buffer, MAX_FILENAME_LENGTH, "%d %s %.2lf\n", i + 1, player->name,
                    (double)player->drink_duration / 1000.0
                );
                strcat(self->text, buffer);
            } else {
                for (size_t j = 0; j < NUM_OPPONENTS; j++) {
                    if (opponents[j]->drink_duration == durations[i]) {
                        snprintf(
                            buffer, MAX_FILENAME_LENGTH, "%d %s %.2lf\n", i + 1, opponents[j]->name,
                            (double)opponents[j]->drink_duration / 1000.0
                        );
                        strcat(self->text, buffer);
                    }
                }
            }
        }
    }
}
