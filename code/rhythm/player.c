#include "player.h"

#include <string.h>

static void player_next_event_func(const SimfileEvent* event, void* arg);

void player_init(Player* player, PlayerType type, const SimfileInputTrackerInterface* input_interface) {
    player->type = type;
    simfile_input_tracker_init(&player->input_tracker, input_interface);
    // TODO: initialize color , stats, etc
    memset(player->result_totals, 0, sizeof(player->result_totals));
}

void player_load_track(Player* player, Track* track) {
    simfile_input_tracker_reset(&player->input_tracker);
    simfile_playback_push_callback(&track->playback, player_next_event_func, player);
    simfile_input_tracker_set_track(&player->input_tracker, track);
}

void player_update(Player* player) {
    player->current_result = simfile_input_tracker_update(&player->input_tracker);
    player->result_totals[player->current_result.type] += 1;
    // TODO: score, combo, etc
}

void player_reset(Player* player) {
    simfile_input_tracker_reset(&player->input_tracker);
}

// triggered when the next event from the simfile context is triggered
static void player_next_event_func(const SimfileEvent* event, void* arg) {
    Player* player = (Player*)arg;
    simfile_input_tracker_enqueue(&player->input_tracker, event);
}