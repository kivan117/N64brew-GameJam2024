#pragma once

#include <stdint.h>

#define SIMFILE_MAX_COLUMN_COUNT 8

typedef enum {
    SIMFILE_EVENT_NONE,
    SIMFILE_EVENT_TAP,
    SIMFILE_EVENT_HOLD,
    SIMFILE_EVENT_ROLL,
    SIMFILE_EVENT_MINE
} SimfileEventType;

typedef struct {
    float time;
    uint16_t duration;
    uint8_t type;
    uint8_t columns;
} SimfileEvent;
