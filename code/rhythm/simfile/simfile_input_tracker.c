#include "simfile_input_tracker.h"

#include <libdragon.h>

#include <float.h>
#include <math.h>
#include <string.h>

// https://www.reddit.com/r/DanceDanceRevolution/comments/4ay7kh/marvelousperfect_timing_windows/
static const float default_time_windows[INPUT_TRACKER_RESULT_COUNT] = {
    // Stepmaina time in ms
    // 21.5f, 43.0f, 102.0f, 135.0f, 180.0f

    // Stepmaina time in seconds
    0.0215f, 0.043f, 0.102f, 0.135f, 0.180f // hard / medium?
    //0.064f, 0.096f, 0.204f, 0.270f, 0.360f // medium / easy?
};

static const SimfileInputTrackerButton default_button_to_column_map[SIMFILE_MAX_COLUMN_COUNT] = {
    SIMFILE_INPUT_TRACKER_BUTTON_A, SIMFILE_INPUT_TRACKER_BUTTON_B, SIMFILE_INPUT_TRACKER_BUTTON_L, SIMFILE_INPUT_TRACKER_BUTTON_R
};

void simfile_input_tracker_init(SimfileInputTracker* tracker, const SimfileContext* context, const SimfileInputTrackerInterface* input_interface){
    tracker->context = context;
    memcpy(&tracker->input_interface, input_interface, sizeof(SimfileInputTrackerInterface));
    memcpy(tracker->time_windows, default_time_windows, sizeof(default_time_windows));

    simfile_input_tracker_set_button_to_column_map(tracker, default_button_to_column_map, SIMFILE_MAX_COLUMN_COUNT);
    simfile_input_tracker_reset(tracker);
}

void simfile_input_tracker_set_button_to_column_map(SimfileInputTracker* tracker, const SimfileInputTrackerButton* default_button_to_column_map, uint32_t count) {
    //TODO: assert that this matches the column count in the simfile?
    memcpy(tracker->button_to_column_map, default_button_to_column_map, count * sizeof(SimfileInputTrackerButton));
    tracker->column_count = count;
}

void simfile_input_tracker_reset(SimfileInputTracker* tracker) {
    tracker->event_index = 0;
    simfile_ring_buffer_init(&tracker->event_buffer);
}

void simfile_input_tracker_enqueue(SimfileInputTracker* tracker, const SimfileEvent* event) {
    if (tracker->event_buffer.count == 0) {
        tracker->event_column_mask = 0;
    }

    simfile_event_buffer_push_back(&tracker->event_buffer, event);
}

static void simfile_input_current_event_complete(SimfileInputTracker* tracker) {
    simfile_event_buffer_pop_front(&tracker->event_buffer);
    tracker->event_index += 1;
    tracker->event_column_mask = 0;
}

static SimfileInputTrackerResultType simfile_input_tracker_update_tap_event(SimfileInputTracker* tracker, const SimfileEvent* current_event, int column) {
    // determine the result time
    for (uint32_t r = 0; r < INPUT_TRACKER_RESULT_COUNT; r++) {
        float event_min = current_event->time - tracker->time_windows[r];
        float event_max = current_event->time + tracker->time_windows[r];
        if ( tracker->context->current_time >= event_min && tracker->context->current_time <= event_max) {
            // flip the bit for this column
            tracker->event_column_mask |= (1 << column);

            // did we complete the event?
            if (tracker->event_column_mask == current_event->columns) {
                simfile_input_current_event_complete(tracker);
                return (SimfileInputTrackerResultType)r;
            }
        }
    }

    return INPUT_TRACKER_RESULT_NONE;
}

SimfileInputTrackerResult simfile_input_tracker_update(SimfileInputTracker* tracker) {
    SimfileInputTrackerResult result = {tracker->event_index , INPUT_TRACKER_RESULT_NONE};
    const SimfileEvent* current_event = simfile_event_buffer_front(&tracker->event_buffer);

    if (current_event == NULL) {
        return result;
    }

    // are we too early to process this event?
    const float min_time = current_event->time - tracker->time_windows[INPUT_TRACKER_RESULT_BOO];
    if (tracker->context->current_time < min_time) {
        return result;
    }

    // did we miss the timing window for this event?
    const float current_event_max_time = current_event->time + tracker->time_windows[INPUT_TRACKER_RESULT_BOO];
    float next_event_min_time = FLT_MAX;
    const SimfileEvent* next_event = simfile_event_buffer_get_at_index(&tracker->event_buffer, 1);
    if (next_event) {
        next_event_min_time = next_event->time - tracker->time_windows[INPUT_TRACKER_RESULT_BOO];
    }

    const float miss_time = next_event_min_time < current_event_max_time ? next_event_min_time: current_event_max_time;

    if (tracker->context->current_time >current_event->time && tracker->context->current_time >= miss_time) {
        result.type = INPUT_TRACKER_RESULT_MISS;
        simfile_input_current_event_complete(tracker);
        return result;
    }

    for (uint32_t i = 0; i < tracker->column_count; i++) {
        // for now we assume that all events are simple taps.
        // when we support more we will need to add additional logic here for different update methods
        if ((current_event->columns & (1 << i)) != 0) {
            int pressed = tracker->input_interface.button_was_pressed(tracker->button_to_column_map[i], tracker->input_interface.callback_arg);

            if (pressed) {
                result.type = simfile_input_tracker_update_tap_event(tracker, current_event, i);
            }
        }
    }

    return result;
}