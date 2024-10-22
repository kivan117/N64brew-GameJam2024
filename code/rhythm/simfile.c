#include "simfile.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

void simfile_init_debug(Simfile* simfile, float total_time, float starting_time_offset, uint32_t event_count, SimfileEvent* events) {
    simfile->total_time = total_time;
    simfile->starting_time_offset = starting_time_offset;
    simfile->event_count = event_count;
    simfile->events = events;
}

#define SIMFILE_INFO_SIZE 12

int simfile_open(Simfile* simfile, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        simfile->event_count = 0;
        simfile->events = NULL;
        return 0;
    }

    fread(simfile, SIMFILE_INFO_SIZE, 1, f);
    simfile->events = malloc(sizeof(SimfileEvent) * simfile->event_count);
    fread(simfile->events, sizeof(SimfileEvent), simfile->event_count, f);

    fclose(f);

    return 1;
}

void simfile_uninit(Simfile* simfile) {
    if (simfile->events) {
        free(simfile->events);
    }
}

void simfile_context_init(SimfileContext* context, Simfile* simfile, float event_lead_time) {
    context->event_lead_time = event_lead_time;
    context->simfile = simfile;
    context->callback_entry_count = 0;
    simfile_context_reset(context);
}

void simfile_context_reset(SimfileContext* context) {
    context->current_time = context->simfile->starting_time_offset;
    context->next_event_index = 0;
}

void simfile_context_push_callback(SimfileContext* context, SimfileEventCallback callback, void* arg) {
    if (context->callback_entry_count == SIMFILE_CONTEXT_MAX_CALLBACKS_COUNT) {
        return;
    }

    SimfileEventCallbackEntry* callback_entry = context->callback_entries + context->callback_entry_count++;
    callback_entry->func = callback;
    callback_entry->arg = arg;
}

void simfile_context_update(SimfileContext* context, float time) {
    // the song itself has finished// nothing to do
    if (context->current_time >= context->simfile->total_time) {
        return;
    }

    context->current_time += time;

    // we have dispatched all remaining events nothing more to do
    if (context->next_event_index >= context->simfile->event_count) {
        return;
    }

    const float event_time = context->current_time + context->event_lead_time;
    SimfileEvent* current_event = context->simfile->events + context->next_event_index;

    // dispatch all events that are within the time window
    while (event_time >= current_event->time) {
        for (size_t i = 0; i < context->callback_entry_count; i++) {
            SimfileEventCallbackEntry* callback_entry = context->callback_entries;
            callback_entry->func(current_event, callback_entry->arg);
        }

        context->next_event_index += 1;
        if (context->next_event_index >= context->simfile->event_count) {
            break;
        } else {
            current_event = context->simfile->events + context->next_event_index;
        }
    }
}

int simfile_context_finished(SimfileContext* context) {
    return context->current_time >= context->simfile->total_time;
}