#pragma once

#include "note_results.h"
#include "button_overlay.h"
#include "indicators.h"
#include "loop_info.h"
#include "resources.h"
#include "track.h"
#include "player.h"

#include "simfile/simfile.h"
#include "simfile/simfile_playback.h"
#include "simfile/simfile_input_tracker.h"

typedef struct {
    RhythmResources* resources;
    Track* track;
    Player* player;

    NoteResults note_results;
    ButtonOverlay button_overlay;
    Indicators indicators;
} StaticOverlay;

void static_overlay_init(StaticOverlay* overlay, const LoopInfo* loop, Track* track, Player* player, RhythmResources* resources);
void static_overlay_uninit(StaticOverlay* overlay);
void static_overlay_tick(StaticOverlay* overlay, float deltatime);
void static_overlay_reset(StaticOverlay* overlay);