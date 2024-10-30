#pragma once

#include <libdragon.h>

#include "simfile/simfile_input_tracker.h"

typedef enum {
    RHYTHM_SPRITE_INDICATOR,
    RHYTHM_SPRITE_BUTTON_A,
    RHYTHM_SPRITE_BUTTON_B,
    RHYTHM_SPRITE_BUTTON_L,
    RHYTHM_SPRITE_BUTTON_R,
    RHYTHM_SPRITE_BUTTON_Z,
    RHYTHM_SPRITE_BUTTON_CR,
    RHYTHM_SPRITE_BUTTON_CL,
    RHYTHM_SPRITE_COUNT
} RhythmSpriteResource;

typedef enum {
    RHYTHM_FONT_BUILTIN,
    RHYTHM_FONT_SQUAREWAVE,
    RHYTHM_FONT_COUNT
} RhythmFontResource;

typedef struct {
    sprite_t* sprites[RHYTHM_SPRITE_COUNT];
    rdpq_font_t* fonts[RHYTHM_FONT_COUNT];
} RhythmResources;

void rhythm_resources_init(RhythmResources* resources);
void rhythm_resources_uninit(RhythmResources* resources);
sprite_t* rhythm_resources_get_sprite_for_button(RhythmResources* resouces, SimfileInputTrackerButton button);
#define rhythm_resources_get_font_id(resources, index) ((index) + 1)