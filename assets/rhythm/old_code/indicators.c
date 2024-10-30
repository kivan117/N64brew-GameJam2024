#include "indicators.h"

#include <string.h>

void indicators_init(Indicators* indicators, float lifetime, const SimfilePlayback* playback, const ButtonOverlay* button_overlay, sprite_t* sprite) {
    indicators->lifetime = lifetime;
    indicators->sprite = sprite;
    indicators->playback = playback;
    indicators->button_overlay = button_overlay;
    indicators_reset(indicators);
}

void indicators_reset(Indicators* indicators) {
    indicators->next_note = 0;
    indicators->head = 0;
    indicators->tail = 0;
}

void indicators_push(Indicators* indicators, const SimfileEvent* event) {
    ButtonOverlayItem* overlay_item = &indicators->button_overlay->overlay_items[indicators->next_note++];
    
    // get next indicator
    Indicator* indicator = &indicators->buffer[indicators->tail ++];
    if (indicators->tail == MAX_INDICATOR_COUNT) {
        indicators->tail = 0;
    }

    indicator->overlay_item = overlay_item;
    indicator->time_remaining = event->time - indicators->playback->current_time;
}

void indicators_tick(Indicators* indicators, float deltatime) {
    rdpq_blitparms_t params;
    memset(&params, 0, sizeof(rdpq_blitparms_t));

    uint32_t current = indicators->head;

    while (current != indicators->tail) {
        Indicator* indicator = &indicators->buffer[current];

        indicator->time_remaining = indicator->time_remaining - deltatime;
        if (indicator->time_remaining > 0) {
            float indicator_scale = 0.5f + ((indicator->time_remaining / indicators->lifetime)* 0.5f) ;

            float indicator_x = indicator->overlay_item->cx - (indicators->sprite->width / 2) * indicator_scale;
            float indicator_y = indicator->overlay_item->cy - (indicators->sprite->height / 2) * indicator_scale;

            params.scale_x = indicator_scale;
            params.scale_y = indicator_scale;

            rdpq_sprite_blit(indicators->sprite, indicator_x, indicator_y, &params);
        } else {
            // if indicator expired pop from buffer
            indicators->head += 1;
            if (indicators->head == MAX_INDICATOR_COUNT) {
                indicators->head = 0;
            }
        }

        current += 1;
        if (current == MAX_INDICATOR_COUNT) {
            current = 0;
        }
    }
}