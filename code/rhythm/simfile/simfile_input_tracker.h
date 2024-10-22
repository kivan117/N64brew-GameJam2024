#pragma once

#include "simfile_context.h"
#include "simfile_event_buffer.h"

typedef enum {
    SIMFILE_INPUT_TRACKER_BUTTON_A          = 1 << 15,
    SIMFILE_INPUT_TRACKER_BUTTON_B          = 1 << 14,
    SIMFILE_INPUT_TRACKER_BUTTON_Z          = 1 << 13,
    SIMFILE_INPUT_TRACKER_BUTTON_START      = 1 << 12,
    SIMFILE_INPUT_TRACKER_BUTTON_DPAD_UP    = 1 << 11,
    SIMFILE_INPUT_TRACKER_BUTTON_DPAD_DOWN  = 1 << 10,
    SIMFILE_INPUT_TRACKER_BUTTON_DPAD_LEFT  = 1 << 9,
    SIMFILE_INPUT_TRACKER_BUTTON_DPAD_RIGHT = 1 << 8,
    //SIMFILE_INPUT_TRACKER_BUTTON_Y          = 1 << 7,
    //SIMFILE_INPUT_TRACKER_BUTTON_X          = 1 << 6,
    SIMFILE_INPUT_TRACKER_BUTTON_L          = 1 << 5,
    SIMFILE_INPUT_TRACKER_BUTTON_R          = 1 << 4,
    SIMFILE_INPUT_TRACKER_BUTTON_C_UP       = 1 << 3,
    SIMFILE_INPUT_TRACKER_BUTTON_C_DOWN     = 1 << 2,
    SIMFILE_INPUT_TRACKER_BUTTON_C_LEFT     = 1 << 1,
    SIMFILE_INPUT_TRACKER_BUTTON_C_RIGHT    = 1 << 0
} SimfileInputTrackerButton;

typedef enum {
    INPUT_TRACKER_RESULT_MARVELOUS,
    INPUT_TRACKER_RESULT_PERFECT,
    INPUT_TRACKER_RESULT_GREAT,
    INPUT_TRACKER_RESULT_GOOD,
    INPUT_TRACKER_RESULT_BOO,

    INPUT_TRACKER_RESULT_COUNT,

    INPUT_TRACKER_RESULT_NONE,
    INPUT_TRACKER_RESULT_MISS,
} SimfileInputTrackerResult;

typedef struct {
    const SimfileContext* context;
    float time_windows[INPUT_TRACKER_RESULT_COUNT];
    SimfileInputTrackerButton button_to_column_map[SIMFILE_MAX_COLUMN_COUNT];
    uint32_t column_count;
    SimfileEventBuffer event_buffer;
    uint32_t controller_port;
} SimfileInputTracker;

void simfile_input_tracker_init(SimfileInputTracker* tracker, const SimfileContext* context, uint32_t controller_port);

/** Ensure that this method is called _after_ simfile_context_update */
SimfileInputTrackerResult simfile_input_tracker_update(SimfileInputTracker* tracker);
void simfile_input_tracker_enqueue(SimfileInputTracker* tracker, const SimfileEvent* event);