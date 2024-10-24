#pragma once

#include "button_overlay.h"
#include "simfile/simfile_context.h"

#include <libdragon.h>
#include <stdint.h>

#define MAX_INDICATOR_COUNT 5
#define DEFAULT_INDICATOR_LIFETIME 0.3f

typedef struct {
    ButtonOverlayItem* overlay_item;
    float time_remaining;
} Indicator;

typedef struct {
    float lifetime;
    sprite_t* sprite;
    size_t next_note;
    const SimfileContext* context;
    const ButtonOverlay* button_overlay;
    Indicator indicators[MAX_INDICATOR_COUNT];
} Indicators;

void indicators_init(Indicators* indicators, float lifetime, const SimfileContext* context, const ButtonOverlay* button_overlay, sprite_t* sprite);
void indicators_reset(Indicators* indicators);

void indicators_push(Indicators* indicators, const SimfileEvent* event);
void indicators_tick(Indicators* indicators, float deltatime);
