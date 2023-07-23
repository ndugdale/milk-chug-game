#include "update.h"

#include <SDL2/SDL.h>

#include "game.h"
#include "input.h"

void gameUpdate(Game* self) {
    while (!isInputEventQueueEmpty(self->inputEventQueue)) {
        InputEvent event = dequeueInputEvent(self->inputEventQueue);

        switch (event) {
            case EVENT_DRINK:
                SDL_Log("DRINK");
                break;
            default:
                break;
        }
    }
}
