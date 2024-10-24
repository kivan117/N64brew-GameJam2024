#pragma once

#include <stdint.h>

#define SIMFILE_MAX_COLUMN_COUNT 16

#define SIMFILE_DEFAULT_COLUMN_COUNT 4

typedef enum {
    SIMFILE_EVENT_NONE,
    SIMFILE_EVENT_TAP,
    SIMFILE_EVENT_HOLD_START,
    SIMFILE_EVENT_HOLD_END,
    SIMFILE_EVENT_ROLL_START,
    SIMFILE_EVENT_ROLL_END,
    SIMFILE_EVENT_MINE
} SimfileEventType;

typedef struct {
    float time;
    uint16_t type;
    uint16_t columns;
} SimfileEvent;
