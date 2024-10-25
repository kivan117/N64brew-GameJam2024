#pragma once

#include "simfile/simfile_input_tracker.h"
#include "track.h"

typedef struct {
    SimfileInputTracker input_tracker;
    SimfileInputTrackerResult current_result;
} Player;

void player_init(Player* player, Track* track);
void player_reset(Player* player);
void player_update(Player* player);