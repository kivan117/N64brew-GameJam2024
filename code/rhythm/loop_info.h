#pragma once

#include "simfile/simfile_input_tracker.h"

typedef struct {
    const char* name;
    const char* wav_file;
    const char* simfile;
    const char* layout;
    SimfileInputTrackerButton column_to_button_map[SIMFILE_TRACKER_DEFAULT_COLUMN_COUNT];
} LoopInfo;