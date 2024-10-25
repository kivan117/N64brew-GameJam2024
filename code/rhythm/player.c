#include "player.h"

static int player_controller_get_button_pressed(int button, void* arg);
static void simfile_next_event_func(const SimfileEvent* event, void* arg);

void player_init(Player* player, Track* track) {
    simfile_playback_push_callback(&track->playback, simfile_next_event_func, player);

    // todo: this is going to moved out to the human player
    #define TEMP_PLAYER_CONTROLLER_PORT
    SimfileInputTrackerInterface input_interface = {player_controller_get_button_pressed, TEMP_PLAYER_CONTROLLER_PORT};
    simfile_input_tracker_init(&player->input_tracker, &track->playback, &input_interface);
}

void player_update(Player* player) {
    player->current_result = simfile_input_tracker_update(&player->input_tracker);
}

void player_reset(Player* player) {
    simfile_input_tracker_reset(&player->input_tracker);
}


// this will probably be moved out of here as there would be a general routine to support AI player input
int player_controller_get_button_pressed(int button, void* arg) {
    uint32_t port = (uint32_t)arg;
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    return btn.raw & (uint32_t)button;
}

// triggered when the next event from the simfile context is triggered
static void simfile_next_event_func(const SimfileEvent* event, void* arg) {
    Player* player = (Player*)arg;
    simfile_input_tracker_enqueue(&player->input_tracker, event);
}