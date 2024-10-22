#pragma once

#include <stddef.h>
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

typedef struct {
    /** The total time of the simfile. */
    float total_time;

    /** defines an offset to the current time of a context that is evaluating this simfile. */
    float starting_time_offset;
    uint32_t event_count;
    SimfileEvent* events;
} Simfile;

void simfile_init_debug(Simfile* simfile, float total_time, float starting_time_offset, uint32_t event_count, SimfileEvent* events);
int simfile_open(Simfile* simfile, const char* path);
void simfile_uninit(Simfile* simfile);

