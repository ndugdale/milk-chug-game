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

        for (size_t i = 0; i < NUM_SCOREBOARD_LINES; i++) {
            blit_text(renderer, font, self->lines[i], colour, 10, i * font_height);
        }
    }
}

void scoreboard_destroy(Scoreboard* self) {
    free(self);
}

static void scoreboard_update_text(Scoreboard* self, Player* player, Opponent* const* opponents) {
    if (self->show && !self->text_set) {
        self->text_set = true;

        char buffer[MAX_SCOREBOARD_LINE_CHARS];
        snprintf(buffer, MAX_FILENAME_LENGTH, "1 %s %.2lf", player->name, (double)player->drink_duration / 1000.0);
        strcpy(self->lines[0], buffer);
        SDL_Log(self->lines[0]);

        for (size_t i = 0; i < NUM_OPPONENTS; i++) {
            char buffer[MAX_SCOREBOARD_LINE_CHARS];
            snprintf(buffer, MAX_FILENAME_LENGTH, "%d %s %.2lf", i + 2, opponents[i]->name, (double)opponents[i]->drink_duration / 1000.0);
            strcpy(self->lines[i + 1], buffer);
            SDL_Log(self->lines[i + 1]);
        }
    }
}
