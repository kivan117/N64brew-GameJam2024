#pragma once

#include "note_results.h"
#include "button_overlay.h"
#include "indicators.h"
#include "loop_info.h"
#include "resources.h"
#include "track.h"

#include "simfile/simfile.h"
#include "simfile/simfile_playback.h"
#include "simfile/simfile_input_tracker.h"

typedef struct {
    RhythmResources* resources;
    Track* track;
    int is_loaded; // TODO: is this needed in overlay?

    // TODO: this will move to player
    SimfileInputTracker tracker;

    // UI
    NoteResults note_results;
    ButtonOverlay button_overlay;
    Indicators indicators;
} StaticOverlay;

void static_overlay_init(StaticOverlay* static_overlay, Track* track, RhythmResources* resources);
void static_overlay_uninit(StaticOverlay* static_overlay);
void static_overlay_tick(StaticOverlay* static_overlay, float deltatime);

void static_overlay_load_loop(StaticOverlay* static_overlay, const LoopInfo* loop);
void static_overlay_reset(StaticOverlay* static_overlay);