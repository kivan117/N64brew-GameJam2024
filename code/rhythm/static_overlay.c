#include "static_overlay.h"

#include <libdragon.h>

static void overlay_next_event_func(const SimfileEvent* event, void* arg);

void static_overlay_init(StaticOverlay* overlay, const LoopInfo* loop, Track* track, Player* player, RhythmResources* resources){
    overlay->resources = resources;

    // track and player not used until update
    overlay->track = track;
    overlay->player = player;

    simfile_playback_push_callback(&track->playback, overlay_next_event_func, overlay);

    button_overlay_init(&overlay->button_overlay, resources);
    button_overlay_open_f(&overlay->button_overlay, loop->layout);
    note_results_init(&overlay->note_results, resources->fonts[RHYTHM_FONT_EVENT_RESULT], rhythm_resources_get_font_id(resources, RHYTHM_FONT_EVENT_RESULT));
    indicators_init(&overlay->indicators, DEFAULT_INDICATOR_LIFETIME, &overlay->track->playback, &overlay->button_overlay, overlay->resources->sprites[RHYTHM_SPRITE_INDICATOR]);
}


void static_overlay_uninit(StaticOverlay* overlay) {
    button_overlay_uninit(&overlay->button_overlay);
}

void static_overlay_tick(StaticOverlay* overlay, float deltatime) {
    const SimfileInputTrackerResult* player_result = &overlay->player->current_result;
    if (player_result->type <= INPUT_TRACKER_RESULT_BOO) {
        ButtonOverlayItem* overlay_item = &overlay->button_overlay.overlay_items[player_result->event_index];
        note_results_push(&overlay->note_results, overlay_item->cx, overlay_item->cy, player_result->type);
    }

    note_results_update(&overlay->note_results, deltatime);
    indicators_tick(&overlay->indicators, deltatime);
    button_overlay_draw(&overlay->button_overlay);
    note_results_draw(&overlay->note_results);
}

// triggered when the next event from the simfile context is triggered
static void overlay_next_event_func(const SimfileEvent* event, void* arg) {
    StaticOverlay* overlay = (StaticOverlay*)arg;
    indicators_push(&overlay->indicators, event);
}

void static_overlay_reset(StaticOverlay* overlay) {
    indicators_reset(&overlay->indicators);
    note_results_reset(&overlay->note_results);
}