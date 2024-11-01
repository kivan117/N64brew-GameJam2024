#pragma once

#include "simfile/metronome.h"
#include "vec2.h"

#include <libdragon.h>

typedef struct {
    sprite_t* sprite;
    Vec2 position;
} TargetIndicator;

void target_indicator_init(TargetIndicator* indicator, sprite_t* sprite, const Vec2* position);
void target_indicator_tick(TargetIndicator* indicator, Metronome* metronome);

