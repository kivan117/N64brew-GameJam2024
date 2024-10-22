#include "simfile_context.h"

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
