#include "simfile.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <libdragon.h>

void simfile_init(Simfile* simfile) {
    memset(simfile, 0, sizeof(Simfile));
}

void simfile_init_debug(Simfile* simfile, float total_time, float starting_time_offset, uint32_t event_count, SimfileEvent* events) {
    simfile->total_time = total_time;
    simfile->starting_time_offset = starting_time_offset;
    simfile->event_count = event_count;
    simfile->events = events;
}

#define SIMFILE_INFO_SIZE 12

int simfile_open(Simfile* simfile, const char* path) {
    simfile_uninit(simfile);

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

    for (uint32_t i = 0; i < simfile->event_count; i++) {
        debugf("%lu: %f, %lu\n", i, simfile->events[i].time, (uint32_t)simfile->events[i].columns);
    }

    return 1;
}

void simfile_uninit(Simfile* simfile) {
    if (simfile->events) {
        free(simfile->events);
    }
}