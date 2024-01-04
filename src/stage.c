#include "stage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>

#include "events.h"
#include "font.h"
#include "game.h"
#include "opponent.h"
#include "player.h"
#include "render.h"
#include "texture.h"

static void stage_tick(Stage* self);
static bool opponents_are_finished(Opponent* const* opponents);

Stage* stage_create(
    AudioManager* audio_manager, FontManager* font_manager, TextureManager* texture_manager,
    Player* player, const char** opponent_names, const char** opponent_texture_ids,
    const char* background_texture_id, uint64_t min_drink_duration, uint64_t max_drink_duration
) {
    Stage* self = malloc(sizeof(Stage));

    self->background = texture_manager_get(texture_manager, background_texture_id);
    self->player = player;
    self->min_drink_duration = min_drink_duration;
    self->max_drink_duration = max_drink_duration;
    self->is_complete = false;

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        const uint64_t range = self->max_drink_duration - self->min_drink_duration;
        uint64_t drink_duration = self->min_drink_duration + (rand() % range);

        const int64_t shift_x = (i - 1) - ((i <= 1) ? 1 : 0);
        int64_t x_position = (BACKGROUND_WIDTH - PLAYER_SPRITE_WIDTH) / 2 + PLAYER_SPRITE_WIDTH * shift_x;
        int64_t y_position = (BACKGROUND_HEIGHT + PLAYER_SPRITE_HEIGHT) / 2;

        self->opponents[i] = opponent_create(
            texture_manager, opponent_names[i], opponent_texture_ids[i],
            drink_duration, x_position, y_position
        );
    }

    const int64_t countdown_x = (BACKGROUND_WIDTH - COUNTDOWN_SPRITE_WIDTH) / 2;
    const int64_t countdown_y = (BACKGROUND_HEIGHT - COUNTDOWN_SPRITE_HEIGHT - 2 * PLAYER_SPRITE_HEIGHT) / 2;
    self->countdown = countdown_create(audio_manager, texture_manager, countdown_x, countdown_y);
    self->countdown_complete = false;

    return self;
}

void stage_update(Stage* self, Event event) {
    countdown_update(self->countdown, event);
    if (!self->countdown_complete && self->countdown->sprite == COUNTDOWN_GO) {
        self->countdown_complete = true;
        player_reset(self->player);
        for (size_t i = 0; i < NUM_OPPONENTS; i++) {
            opponent_reset(self->opponents[i]);
        }
    }

    if (self->countdown_complete) {
        player_update(self->player, event);
        for (size_t i = 0; i < NUM_OPPONENTS; i++) {
            opponent_update(self->opponents[i], event);
        }
    }

    switch (event) {
        case EVENT_TICK:
            stage_tick(self);
        default:
            break;
    }
}

void stage_render(Stage* self, SDL_Renderer* renderer, SDL_Window* window) {
    blit_background(
        renderer, window, self->background, (double)RENDERER_SCALE_FACTOR,
        BACKGROUND_WIDTH, BACKGROUND_HEIGHT
    );

    player_render(self->player, renderer, window);

    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_render(self->opponents[i], renderer, window);
    }

    countdown_render(self->countdown, renderer, window);
}

void stage_destroy(Stage* self) {
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        opponent_destroy(self->opponents[i]);
    }
    countdown_destroy(self->countdown);
    free(self);
}

Stage* stage_create_from_id(
    AudioManager* audio_manager, FontManager* font_manager,
    TextureManager* texture_manager, Player* player, uint8_t stage_id
) {
    Stage* stage;

    switch (stage_id) {
        case 0:
            const char* opponent_names_0[] = {
                "Milo Dynaseur",
                "Nessie Quick",
                "Luke Toes",
                "Hammad To'swollo"};

            const char* opponent_texture_ids_0[] = {
                "opponent_0_0",
                "opponent_0_1",
                "opponent_0_2",
                "opponent_0_3"};

            const char* background_texture_id_0 = "stage_0";

            stage = stage_create(
                audio_manager, font_manager, texture_manager,
                player, opponent_names_0, opponent_texture_ids_0,
                background_texture_id_0, 12'000, 16'000
            );
            break;

        case 1:
            const char* opponent_names_1[] = {
                "Mama Reigland",
                "Pa Sterise",
                "Atty Fishal-Flavour",
                "Cole Oid"};

            const char* opponent_texture_ids_1[] = {
                "opponent_1_0",
                "opponent_1_1",
                "opponent_1_2",
                "opponent_1_3"};

            const char* background_texture_id_1 = "stage_1";

            stage = stage_create(
                audio_manager, font_manager, texture_manager,
                player, opponent_names_1, opponent_texture_ids_1,
                background_texture_id_1, 10'000, 14'000
            );
            break;

        case 2:
            const char* opponent_names_2[] = {
                "Con Denzt",
                "Phoebe Tacaisen",
                "Mills Keys",
                "Eva Porated"};

            const char* opponent_texture_ids_2[] = {
                "opponent_2_0",
                "opponent_2_1",
                "opponent_2_2",
                "opponent_2_3"};

            const char* background_texture_id_2 = "stage_2";

            stage = stage_create(
                audio_manager, font_manager, texture_manager,
                player, opponent_names_2, opponent_texture_ids_2,
                background_texture_id_2, 8'000, 11'000
            );
            break;

        case 3:
            const char* opponent_names_3[] = {
                "Chu Ging",
                "Bjorn Tudrink",
                "Grace Fullswig",
                "Tia Tees"};

            const char* opponent_texture_ids_3[] = {
                "opponent_3_0",
                "opponent_3_1",
                "opponent_3_2",
                "opponent_3_3"};

            const char* background_texture_id_3 = "stage_3";

            stage = stage_create(
                audio_manager, font_manager, texture_manager,
                player, opponent_names_3, opponent_texture_ids_3,
                background_texture_id_3, 6'000, 8'000
            );
            break;

        case 4:
            const char* opponent_names_4[] = {
                "Jim",
                "Capt. Taika Sip",
                "Org Anik",
                "Homer J'Naisd"};

            const char* opponent_texture_ids_4[] = {
                "opponent_4_0",
                "opponent_4_1",
                "opponent_4_2",
                "opponent_4_3"};

            const char* background_texture_id_4 = "stage_4";

            stage = stage_create(
                audio_manager, font_manager, texture_manager,
                player, opponent_names_4, opponent_texture_ids_4,
                background_texture_id_4, 5'000, 7'000
            );
            break;

        default:
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "Invalid stage id: %d",
                stage_id
            );

            return stage;
    }
}

static void stage_tick(Stage* self) {
    if (!self->is_complete && self->player->finished && opponents_are_finished(self->opponents)) {
        self->is_complete = true;
    }
}

static bool opponents_are_finished(Opponent* const* opponents) {
    for (size_t i = 0; i < NUM_OPPONENTS; i++) {
        if (!opponents[i]->finished) {
            return false;
        }
    }
    return true;
}
