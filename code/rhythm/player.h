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

/** 
 * This method creates a new player and should be called once per game
 * Note: Input interface is copied and does not need to exist beyond the call to this function. 
 * */
void player_init(Player* player, PlayerType type, const SimfileInputTrackerInterface* input_interface);

/**
 * This method sets up playback callbacks for the input tracker ans should be called every time a new track is loaded.
 * This method will implicitly call \ref player_reset
 */
void player_load_track(Player* player, Track* track);

/** Call this method when the current track is reset*/
void player_reset(Player* player);

void player_update(Player* player);