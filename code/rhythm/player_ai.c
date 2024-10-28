#include "player_ai.h"

#include <float.h>
#include <stdlib.h>

#define PLAYER_AI_MISS_NEXT_EVENT (3600.0f)

static const float ai_easy_weights[INPUT_TRACKER_RESULT_COUNT] = {
    0.15f, //perfect 
    0.25f, // great
    0.25f, // good
    0.15f, // boo
    // 0.20 miss
};

static const float ai_medium_weights[INPUT_TRACKER_RESULT_COUNT] = {
    0.25f, //perfect 
    0.35f, // great
    0.15f, // good
    0.10f, // boo
    // 0.15 miss
};

static const float ai_hard_weights[INPUT_TRACKER_RESULT_COUNT] = {
    0.60f, //perfect 
    0.25f, // great
    0.05f, // good
    0.05f, // boo
    // 0.05 miss
};

static int player_ai_controller_get_button_pressed(int button, void* arg);
static void reset_ai_event_schedule(PlayerAi* player);

void player_ai_init(PlayerAi* player, AiDiff difficulty) {
    debugf("player_ai_init: %i\n", (int)difficulty);
    SimfileInputTrackerInterface input_interface = {player_ai_controller_get_button_pressed, player};
    player_init(&player->base, PLAYER_TYPE_AI, &input_interface);
    player->base.input_tracker.debug_handle = PLAYER_TYPE_CONTROLLER;

    player->difficulty = difficulty;
    reset_ai_event_schedule(player);

    switch (player->difficulty)
    {
        case DIFF_EASY:
            player->window_weights = ai_easy_weights;
            break;

        case DIFF_MEDIUM:
            player->window_weights = ai_medium_weights;
            break;

        case DIFF_HARD:
            player->window_weights = ai_hard_weights;
            break;
    }
}

void reset_ai_event_schedule(PlayerAi* player) {
    player->previous_event = NULL;
    player->next_button_press_time = -1000;
}

void player_ai_load_track(PlayerAi* player, Track* track) {
    player_load_track(&player->base, track);
    reset_ai_event_schedule(player);
}

void player_ai_reset(PlayerAi* player) {
    player_reset(&player->base);
    reset_ai_event_schedule(player);
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
        player->next_button_press_time = PLAYER_AI_MISS_NEXT_EVENT;
        return;
    }

    // determine the time of the next ai button press by picking a weighted timing window and storing the press time
    float x = ((float)rand()) / RAND_MAX;
    for (int i = 0; i < INPUT_TRACKER_RESULT_COUNT; i++) {
        x -= player->window_weights[i];

        if (x <= 0.0f) {
            player->next_button_press_time = current_event->time - tracker->time_windows[i];
            debugf("AI(%i): queue press button (%i) at: %f\n", (int)player->difficulty, i, player->next_button_press_time);
            return;
        }
    }

    // no event scheduled...the ai will miss this note
    player->next_button_press_time = PLAYER_AI_MISS_NEXT_EVENT;
    debugf("AI(%i): queue miss event at: %f\n", (int)player->difficulty, player->next_button_press_time);
}

int player_ai_controller_get_button_pressed(int button, void* arg) {
    PlayerAi* player = (PlayerAi*)arg;

    if (player->base.input_tracker.playback->current_time > player->next_button_press_time) {
        debugf("AI(%i): Press Button %i (t:%f)\n", (int)player->difficulty, button, player->base.input_tracker.playback->current_time);
        return 1;
    }

    return 0;
}
