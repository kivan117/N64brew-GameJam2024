#pragma once

#include "simfile/simfile_playback.h"
#include "vec2.h"

#include <libdragon.h>

typedef struct {
    sprite_t* sprite;
    Vec2 position;
} PulseSprite;

void pulse_sprite_init(PulseSprite* pulse_sprite, sprite_t* sprite, const Vec2* position);
void pulse_sprite_tick(PulseSprite* pulse_sprite, SimfilePlayback* playback);