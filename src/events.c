#include "events.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

EventQueue* event_queue_create(void) {
    EventQueue* self = (EventQueue*)malloc(sizeof(EventQueue));
    self->front = EMPTY_QUEUE;
    self->rear = EMPTY_QUEUE;

    return self;
}

bool event_queue_is_empty(const EventQueue* self) {
    return (self->front == EMPTY_QUEUE && self->rear == EMPTY_QUEUE);
}

void event_queue_enqueue(EventQueue* self, Event event) {
    if (event_queue_is_empty(self)) {
        self->front = 0;
        self->rear = 0;
    } else {
        self->rear = (self->rear + 1) % MAX_QUEUED_EVENTS;
    }
    self->events[self->rear] = event;
}

Event event_queue_dequeue(EventQueue* self) {
    Event event = self->events[self->front];
    if (self->front == self->rear) {
        self->front = EMPTY_QUEUE;
        self->rear = EMPTY_QUEUE;
    } else {
        self->front = (self->front + 1) % MAX_QUEUED_EVENTS;
    }
    return event;
}

void event_queue_destroy(EventQueue* self) {
    free(self);
}
