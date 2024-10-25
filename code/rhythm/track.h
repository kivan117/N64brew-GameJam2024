#pragma once

#include "simfile/simfile.h"
#include "simfile/simfile_playback.h"

#include <libdragon.h>

/** This struct represents a current simfile playback */
typedef struct {
    wav64_t audio_file;
    int mixer_channel;

    Simfile simfile;
    SimfilePlayback playback;
} Track;

void track_init(Track* track, int mixer_channel, const char* audio_file_asset, const char* simfile_asset, float event_lifetime);
void track_update(Track* track, float time);
void track_reset(Track* track);
void track_unload(Track* track);