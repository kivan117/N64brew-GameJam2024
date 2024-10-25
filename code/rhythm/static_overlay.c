#include "static_overlay.h"

#include <libdragon.h>

void static_overlay_init(StaticOverlay* overlay, Track* track, RhythmResources* resources){
    overlay->resources = resources;
    overlay->track = track;
    overlay->is_loaded = 0;

    button_overlay_init(&overlay->button_overlay, resources);
    note_results_init(&overlay->note_results, resources->fonts[RHYTHM_FONT_EVENT_RESULT], rhythm_resources_get_font_id(resources, RHYTHM_FONT_EVENT_RESULT));
}


void static_overlay_uninit(StaticOverlay* overlay) {
    button_overlay_uninit(&overlay->button_overlay);
    overlay->is_loaded = 0;
}

void static_overlay_tick(StaticOverlay* overlay, float deltatime) {
    note_results_update(&overlay->note_results, deltatime);
    
    
    // TODO: this is going to move into player
    SimfileInputTrackerResult result = simfile_input_tracker_update(&overlay->tracker);

    if (result.type <= INPUT_TRACKER_RESULT_BOO) {
        ButtonOverlayItem* overlay_item = &overlay->button_overlay.overlay_items[result.event_index];
        note_results_push(&overlay->note_results, overlay_item->cx, overlay_item->cy, result.type);
    }
    // ----
    
    indicators_tick(&overlay->indicators, deltatime);
    button_overlay_draw(&overlay->button_overlay);
    note_results_draw(&overlay->note_results);
}


// triggered when the next event from the simfile context is triggered
static void simfile_next_event_func(const SimfileEvent* event, void* arg) {
    StaticOverlay* overlay = (StaticOverlay*)arg;
    indicators_push(&overlay->indicators, event);

    // TODO: this is probably going to be moved out of here to player
    simfile_input_tracker_enqueue(&overlay->tracker, event);
}

// this will probably be moved out of here as there would be a general routine to support AI player input
static int player_controller_get_button_pressed(int button, void* arg) {
    uint32_t port = (uint32_t)arg;
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    return btn.raw & (uint32_t)button;
}

void static_overlay_load_loop(StaticOverlay* overlay, const LoopInfo* loop) {

    // TODO: this is going to be partially broken out into player
    simfile_playback_push_callback(&overlay->track->playback, simfile_next_event_func, overlay);

    SimfileInputTrackerInterface input_interface = {player_controller_get_button_pressed, 0};
    simfile_input_tracker_init(&overlay->tracker, &overlay->track->playback, &input_interface);
    simfile_input_tracker_set_button_to_column_map(&overlay->tracker, loop->column_to_button_map, SIMFILE_DEFAULT_COLUMN_COUNT);
    // ----

    // intialize UI
    button_overlay_open_f(&overlay->button_overlay, loop->layout);
    note_results_reset(&overlay->note_results);
    indicators_init(&overlay->indicators, DEFAULT_INDICATOR_LIFETIME, &overlay->track->playback, &overlay->button_overlay, overlay->resources->sprites[RHYTHM_SPRITE_INDICATOR]);

    overlay->is_loaded = 1;
}

void static_overlay_reset(StaticOverlay* overlay) {
    indicators_reset(&overlay->indicators);
    note_results_reset(&overlay->note_results);

    // TODO: This will move out to player
    simfile_input_tracker_reset(&overlay->tracker);
}