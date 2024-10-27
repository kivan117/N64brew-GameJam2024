#include "player_ai.h"

#include <float.h>

static int player_ai_controller_get_button_pressed(int button, void* arg);


void player_ai_init(PlayerAi* player, AiDiff difficulty) {
    player->difficulty = difficulty;
    player->previous_event = NULL;
    player->next_button_press_time = FLT_MIN;

    SimfileInputTrackerInterface input_interface = {player_ai_controller_get_button_pressed, player};
    player_init(&player->base, PLAYER_TYPE_AI, &input_interface);
}

void player_ai_reset(PlayerAi* player, Track* track) {
    player_reset(&player->base, track);
    player->previous_event = NULL;
}

void player_ai_update(PlayerAi* player) {
    player_update(&player->base);

    SimfileInputTracker* tracker = &player->base.input_tracker;
    const SimfileEvent* current_event = simfile_input_tracker_get_current_event(tracker);

    // if there is not a new event being tracked then we dont have to do anything here
    if (current_event == player->previous_event) {
        return;
    }

    player->previous_event = current_event;

    // in this case we have exhaused available inputs in the input tracker
    if (current_event == NULL) {
        player->next_button_press_time = FLT_MIN;
        return;
    }

    // determine the time of the next ai button press by picking a weighted timing window and storing the press time
    // TODO: handle mcase where multiple presses are required
    player->next_button_press_time = current_event->time - tracker->time_windows[INPUT_TRACKER_RESULT_GREAT];
    debugf("AI(%i): queue press button at: %f\n", (int)player->difficulty, player->next_button_press_time);
}

int player_ai_controller_get_button_pressed(int button, void* arg) {
    PlayerAi* player = (PlayerAi*)arg;

    if (player->base.input_tracker.playback->current_time >= player->next_button_press_time) {
        debugf("AI(%i): Press Button (t:%f)\n", (int)player->difficulty, player->base.input_tracker.playback->current_time);
        return 1;
    }

    return 0;
}
