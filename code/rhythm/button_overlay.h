#pragma once

#include "resources.h"

#include <libdragon.h>

#include <stdint.h>

typedef struct {
    float cx, cy;
    sprite_t* button1;
    sprite_t* button2;
} ButtonOverlayItem;

typedef struct {
    RhythmResources* resources;
    ButtonOverlayItem* overlay_items;
    uint32_t overlay_item_count;
} ButtonOverlay;

void button_overlay_init(ButtonOverlay* overlay, RhythmResources* resources);
void button_overlay_uninit(ButtonOverlay* overlay);
void button_overlay_load_debug(ButtonOverlay* overlay, ButtonOverlayItem* overlay_items, uint32_t item_count);
int button_overlay_open_f(ButtonOverlay* overlay, const char* path);
void button_overlay_draw(ButtonOverlay* overlay);
