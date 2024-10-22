#pragma once

#include "simfile.h"

#include <stdint.h>

#define SIMFILE_EVENT_BUFFER_SIZE 10

typedef struct {
    uint32_t head, tail, count;
    const SimfileEvent* events[SIMFILE_EVENT_BUFFER_SIZE];
} SimfileEventBuffer;

void simfile_ring_buffer_init(SimfileEventBuffer* event_buffer);
int simfile_event_buffer_push_back(SimfileEventBuffer* event_buffer, const SimfileEvent* event);
const SimfileEvent* simfile_event_buffer_front(SimfileEventBuffer* event_buffer);
void simfile_event_buffer_pop_front(SimfileEventBuffer* event_buffer);
