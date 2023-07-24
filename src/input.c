#include "input.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

InputEventQueue* input_event_queue_create(void) {
    InputEventQueue* self = (InputEventQueue*)calloc(1, sizeof(InputEventQueue));
    self->front = EMPTY_QUEUE;
    self->rear = EMPTY_QUEUE;

    return self;
}

bool input_event_queue_is_empty(InputEventQueue* self) {
    return (self->front == EMPTY_QUEUE && self->rear == EMPTY_QUEUE);
}

void input_event_queue_enqueue(InputEventQueue* self, InputEvent event) {
    if (input_event_queue_is_empty(self)) {
        self->front = 0;
        self->rear = 0;
    } else {
        self->rear = (self->rear + 1) % MAX_QUEUED_INPUT_EVENTS;
    }
    self->events[self->rear] = event;
}

InputEvent input_event_queue_dequeue(InputEventQueue* self) {
    InputEvent event = self->events[self->front];
    if (self->front == self->rear) {
        self->front = EMPTY_QUEUE;
        self->rear = EMPTY_QUEUE;
    } else {
        self->front = (self->front + 1) % MAX_QUEUED_INPUT_EVENTS;
    }
    return event;
}

void input_event_queue_destroy(InputEventQueue* self) {
    free(self);
}
