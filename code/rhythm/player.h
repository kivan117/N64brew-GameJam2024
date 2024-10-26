#pragma once

#include "simfile/simfile_input_tracker.h"
#include "track.h"

typedef enum {
    PLAYER_TYPE_CONTROLLER,
    PLAYER_TYPE_AI
} PlayerType;

typedef struct {
    PlayerType type;
    SimfileInputTracker input_tracker;
    SimfileInputTrackerResult current_result;

    // score
    // combo
    // others?
} Player;

/** Note: Input interface is copied and does not need to exist beyond the call to this function. */
void player_init(Player* player, PlayerType type, const SimfileInputTrackerInterface* input_interface);

/** Note this needs to be called after the track is reloaded and before any call to \ref player_update */
void player_reset(Player* player, Track* track);

void player_update(Player* player);