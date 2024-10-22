#include "simfile_input_tracker.h"

#include <libdragon.h>

#include <math.h>
#include <string.h>

// https://www.reddit.com/r/DanceDanceRevolution/comments/4ay7kh/marvelousperfect_timing_windows/
static const float default_time_windows[INPUT_TRACKER_RESULT_COUNT] = {
    21.5f, 43.0f, 102.0f, 135.0f, 180.0f
};
#define SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT 4
static const SimfileInputTrackerButton default_button_to_column_map[SIMFILE_MAX_COLUMN_COUNT] = {
    SIMFILE_INPUT_TRACKER_BUTTON_A, SIMFILE_INPUT_TRACKER_BUTTON_B, SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R
};

void simfile_input_tracker_init(SimfileInputTracker* tracker, const SimfileContext* context, uint32_t controller_port){
    tracker->context = context;
    tracker->controller_port = controller_port;
    memcpy(tracker->time_windows, default_time_windows, sizeof(default_time_windows));
    simfile_ring_buffer_init(&tracker->event_buffer);
    memcpy(tracker->button_to_column_map, default_button_to_column_map, SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT);
    tracker->column_count = SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT;
}

void simfile_input_tracker_enqueue(SimfileInputTracker* tracker, const SimfileEvent* event) {
    simfile_event_buffer_push_back(&tracker->event_buffer, event);
}

SimfileInputTrackerResult simfile_input_tracker_update(SimfileInputTracker* tracker) {
    const SimfileEvent* current_event = simfile_event_buffer_front(&tracker->event_buffer);

    if (current_event == NULL) {
        return INPUT_TRACKER_RESULT_NONE;
    }

    // are we too early to process this note?
    const float min_time = current_event->time - tracker->time_windows[INPUT_TRACKER_RESULT_BOO];
    if (tracker->context->current_time < min_time) {
        return INPUT_TRACKER_RESULT_NONE;
    }

    // did we miss the timing window for this note?
    const float max_time = current_event->time + tracker->time_windows[INPUT_TRACKER_RESULT_BOO];
    if (tracker->context->current_time > max_time) {
        simfile_event_buffer_pop_front(&tracker->event_buffer);
        return INPUT_TRACKER_RESULT_MISS;
    }

    // check if we are pressed the correct button and determine time window
    const float abs_time = fabs(tracker->context->current_time - current_event->time);
    joypad_buttons_t btn = joypad_get_buttons_pressed(tracker->controller_port);

    for (uint32_t i = 0; i < tracker->column_count; i++) {
        // continue if no note in this column
        if ((current_event->columns & i) == 0) {
            continue;
        }

        // did we press the button required for this note?
        if (btn.raw & tracker->button_to_column_map[i]) {
            simfile_event_buffer_pop_front(&tracker->event_buffer);

            // determine the result time
            for (uint32_t r = 0; r < INPUT_TRACKER_RESULT_COUNT; r++) {
                if (abs_time <= tracker->time_windows[r]) {
                    return (SimfileInputTrackerResult)r;
                }
            }
        }
    }

    return INPUT_TRACKER_RESULT_NONE;
}