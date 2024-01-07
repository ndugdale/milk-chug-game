#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static void load_texture(
    TextureManager* self, SDL_Renderer* renderer, const char* id,
    const char* path
);

TextureManager* texture_manager_create(SDL_Renderer* renderer) {
    TextureManager* self = calloc(1, sizeof(TextureManager));

    // Load player character textures
    load_texture(self, renderer, "player", "images/characters/player.png");

    // Load non-player character textures
    load_texture(self, renderer, "opponent_0_0", "images/characters/opponent_0_0.png");
    load_texture(self, renderer, "opponent_0_1", "images/characters/opponent_0_1.png");
    load_texture(self, renderer, "opponent_0_2", "images/characters/opponent_0_2.png");
    load_texture(self, renderer, "opponent_0_3", "images/characters/opponent_0_3.png");
    load_texture(self, renderer, "opponent_1_0", "images/characters/opponent_1_0.png");
    load_texture(self, renderer, "opponent_1_1", "images/characters/opponent_1_1.png");
    load_texture(self, renderer, "opponent_1_2", "images/characters/opponent_1_2.png");
    load_texture(self, renderer, "opponent_1_3", "images/characters/opponent_1_3.png");
    load_texture(self, renderer, "opponent_2_0", "images/characters/opponent_2_0.png");
    load_texture(self, renderer, "opponent_2_1", "images/characters/opponent_2_1.png");
    load_texture(self, renderer, "opponent_2_2", "images/characters/opponent_2_2.png");
    load_texture(self, renderer, "opponent_2_3", "images/characters/opponent_2_3.png");
    load_texture(self, renderer, "opponent_3_0", "images/characters/opponent_3_0.png");
    load_texture(self, renderer, "opponent_3_1", "images/characters/opponent_3_1.png");
    load_texture(self, renderer, "opponent_3_2", "images/characters/opponent_3_2.png");
    load_texture(self, renderer, "opponent_3_3", "images/characters/opponent_3_3.png");
    load_texture(self, renderer, "opponent_4_0", "images/characters/opponent_4_0.png");
    load_texture(self, renderer, "opponent_4_1", "images/characters/opponent_4_1.png");
    load_texture(self, renderer, "opponent_4_2", "images/characters/opponent_4_2.png");
    load_texture(self, renderer, "opponent_4_3", "images/characters/opponent_4_3.png");

    // Load background textures
    load_texture(self, renderer, "stage_0", "images/backgrounds/stage_0.png");
    load_texture(self, renderer, "stage_1", "images/backgrounds/stage_1.png");
    load_texture(self, renderer, "stage_2", "images/backgrounds/stage_2.png");
    load_texture(self, renderer, "stage_3", "images/backgrounds/stage_3.png");
    load_texture(self, renderer, "stage_4", "images/backgrounds/stage_4.png");

    // Load UI textures
    load_texture(self, renderer, "title", "images/ui/title.png");
    load_texture(self, renderer, "countdown", "images/ui/countdown.png");
    load_texture(self, renderer, "indicator", "images/ui/indicator.png");

    return self;
}

SDL_Texture* texture_manager_get(TextureManager* self, const char* id) {
    for (size_t i = 0; i < self->insert_index; i++) {
        if (strcmp(self->textures[i]->id, id) == 0) {
            return self->textures[i]->texture;
        }
    }
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "No texture with id %s is stored in the texture manager",
        id
    );
    return NULL;
}

void texture_manager_destroy(TextureManager* self) {
    for (size_t i = 0; i < self->insert_index; i++) {
        SDL_DestroyTexture(self->textures[i]->texture);
        free(self->textures[i]);
    }
    free(self);
}

static void load_texture(
    TextureManager* self, SDL_Renderer* renderer, const char* id,
    const char* path
) {
    char resolved_path[MAX_PATH_CHARS];
    get_asset_path(resolved_path, path);

    if (self->insert_index > MAX_NUM_TEXTURES) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load texture from file %s: "
            "texture manager has run out of capacity",
            resolved_path
        );
        exit(1);
    }

    SDL_Log("Loading texture: %s", resolved_path);
    SDL_Texture* texture = IMG_LoadTexture(renderer, resolved_path);

    if (texture == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to load texture from file %s: %s",
            resolved_path,
            SDL_GetError()
        );
        exit(1);
    }

    Texture* insert_item = malloc(sizeof(Texture));
    insert_item->id = id;
    insert_item->texture = texture;

    self->textures[self->insert_index] = insert_item;
    self->insert_index++;
}
