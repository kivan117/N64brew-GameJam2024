#include "indicators.h"

#include <string.h>

void indicators_init(Indicators* indicators, float lifetime, const SimfileContext* context, const ButtonOverlay* button_overlay, sprite_t* sprite) {
    indicators->lifetime = lifetime;
    indicators->sprite = sprite;
    indicators->context = context;
    indicators->button_overlay = button_overlay;

    indicators_reset(indicators);
}

void indicators_reset(Indicators* indicators) {
    memset(&indicators->indicators, 0, sizeof(indicators));
    indicators->next_note = 0;
}

void indicators_push(Indicators* indicators, const SimfileEvent* event) {
    ButtonOverlayItem* overlay_item = &indicators->button_overlay->overlay_items[indicators->next_note++];
    
    // get next indicator
    Indicator* indicator = NULL;
    for (size_t i = 0; i < MAX_INDICATOR_COUNT; i++) {
        if (indicators->indicators[i].time_remaining <= 0) {
            indicator = &indicators->indicators[i];
        }
    }

    if (!indicator) {
        return; // should not happen
    }

    indicator->overlay_item = overlay_item;
    indicator->time_remaining = event->time - indicators->context->current_time;
}

void indicators_tick(Indicators* indicators, float deltatime) {
    rdpq_blitparms_t params;
    memset(&params, 0, sizeof(rdpq_blitparms_t));

    for (size_t i = 0; i < MAX_INDICATOR_COUNT; i++) {
        Indicator* indicator = &indicators->indicators[i];

        if (indicator->time_remaining <= 0) {
            continue;
        }
        
        indicator->time_remaining = indicator->time_remaining - deltatime;

        float indicator_scale = 0.5f + ((indicator->time_remaining / indicators->lifetime)* 0.5f) ;

        float indicator_x = indicator->overlay_item->cx - (indicators->sprite->width / 2) * indicator_scale;
        float indicator_y = indicator->overlay_item->cy - (indicators->sprite->height / 2) * indicator_scale;

        params.scale_x = indicator_scale;
        params.scale_y = indicator_scale;

        rdpq_sprite_blit(indicators->sprite, indicator_x, indicator_y, &params);
    }
}