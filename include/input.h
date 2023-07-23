#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#define DRINK_MILK_INPUT SDL_SCANCODE_X
#define MAX_QUEUED_INPUT_EVENTS 64
#define EMPTY_QUEUE -1

typedef enum {
    EVENT_DRINK,
    NUM_EVENTS
} InputEvent;

typedef struct {
    InputEvent events[MAX_QUEUED_INPUT_EVENTS];
    int front;
    int rear;
} InputEventQueue;

void inputEventQueueInit(InputEventQueue* queue);
bool isInputEventQueueEmpty(InputEventQueue* queue);
void enqueueInputEvent(InputEventQueue* queue, InputEvent event);
InputEvent dequeueInputEvent(InputEventQueue* queue);
