#pragma once

#include "simfile_event.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    /** The total time of the simfile. */
    float total_time;

    /** defines an offset to the current time of a context that is evaluating this simfile. */
    float starting_time_offset;
    uint32_t event_count;
    SimfileEvent* events;
} Simfile;

void simfile_init(Simfile* simfile);
void simfile_init_debug(Simfile* simfile, float total_time, float starting_time_offset, uint32_t event_count, SimfileEvent* events);
int simfile_open(Simfile* simfile, const char* path);
void simfile_uninit(Simfile* simfile);

