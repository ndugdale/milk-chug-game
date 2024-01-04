#include "audio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static void load_effect_audio(AudioManager* self, const char* id, const char* path);
static void load_music_audio(AudioManager* self, const char* id, const char* path);
static Mix_Chunk* audio_manager_get_effect(AudioManager* self, const char* id);
static Mix_Music* audio_manager_get_music(AudioManager* self, const char* id);

AudioManager* audio_manager_create(void) {
    AudioManager* self = (calloc(1, sizeof(AudioManager)));
    self->muted = false;

    // Load audio effects
    load_effect_audio(self, "beep_low", "assets/audio/beep_low.wav");
    load_effect_audio(self, "beep_high", "assets/audio/beep_high.wav");
    load_effect_audio(self, "glug", "assets/audio/glug.wav");

    // Load music audio
    load_music_audio(self, "main_theme", "assets/audio/dreamtune.wav");

    return self;
}

void audio_manager_play_effect(AudioManager* self, const char* id) {
    Mix_Chunk* chunk = audio_manager_get_effect(self, id);
    if (!self->muted && Mix_PlayChannel(-1, chunk, 0) == -1) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to play audio effect %s: %s",
            id,
            Mix_GetError()
        );
    }
}

void audio_manager_play_music(AudioManager* self, const char* id) {
    Mix_Music* music = audio_manager_get_music(self, id);
    if (Mix_PlayMusic(music, -1) != 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to play music audio %s: %s",
            id,
            Mix_GetError()
        );
    }
}

void audio_manager_toggle(AudioManager* self) {
    if (!self->muted) {
        Mix_Pause(-1);
        Mix_PauseMusic();
    } else {
        Mix_ResumeMusic();
    }
    self->muted = !self->muted;
}

void audio_manager_destroy(AudioManager* self) {
    for (size_t i = 0; i < self->effect_insert_index; i++) {
        Mix_FreeChunk(self->effects[i]->chunk);
        free(self->effects[i]);
    }
    for (size_t i = 0; i < self->music_insert_index; i++) {
        Mix_FreeMusic(self->musics[i]->music);
        free(self->musics[i]);
    }
    free(self);
}

static void load_effect_audio(AudioManager* self, const char* id, const char* path) {
    if (self->effect_insert_index > MAX_NUM_AUDIO) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to load audio effect from file %s: "
            "audio manager has run out of capacity",
            path
        );
        exit(1);
    }

    SDL_Log("Loading audio effect: %s", path);
    Mix_Chunk* chunk = Mix_LoadWAV(path);

    if (chunk == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to load audio effect from file %s: %s",
            path,
            Mix_GetError()
        );
        exit(1);
    }

    EffectAudio* insert_item = malloc(sizeof(EffectAudio));
    insert_item->id = id;
    insert_item->chunk = chunk;

    self->effects[self->effect_insert_index] = insert_item;
    self->effect_insert_index++;
}

static void load_music_audio(AudioManager* self, const char* id, const char* path) {
    if (self->music_insert_index > MAX_NUM_AUDIO) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to load music audio from file %s: "
            "audio manager has run out of capacity",
            path
        );
        exit(1);
    }

    SDL_Log("Loading music audio: %s", path);
    Mix_Music* music = Mix_LoadMUS(path);

    if (music == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_AUDIO,
            "Failed to load music audio from file %s: %s",
            path,
            Mix_GetError()
        );
        exit(1);
    }

    MusicAudio* insert_item = malloc(sizeof(insert_item));
    insert_item->id = id;
    insert_item->music = music;

    self->musics[self->music_insert_index] = insert_item;
    self->music_insert_index++;
}

static Mix_Chunk* audio_manager_get_effect(AudioManager* self, const char* id) {
    for (size_t i = 0; i < self->effect_insert_index; i++) {
        if (strcmp(self->effects[i]->id, id) == 0) {
            return self->effects[i]->chunk;
        }
    }
    SDL_LogError(
        SDL_LOG_CATEGORY_AUDIO,
        "No audio effect with id %s is stored in the audio manager",
        id
    );
}

static Mix_Music* audio_manager_get_music(AudioManager* self, const char* id) {
    for (size_t i = 0; i < self->music_insert_index; i++) {
        if (strcmp(self->musics[i]->id, id) == 0) {
            return self->musics[i]->music;
        }
    }
    SDL_LogError(
        SDL_LOG_CATEGORY_AUDIO,
        "No music audio with id %s is stored in the audio manager",
        id
    );
}
