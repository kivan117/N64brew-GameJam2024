#pragma once

#include "resources.h"
#include "simfile/simfile_playback.h"

#include <libdragon.h>

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec2 position;
} EventTrackItem;

#define EVENT_TRACK_BUFFER_SIZE 10

typedef struct {
    Vec2 source_pos, target_pos, direction;
    float speed, lifetime;
    EventTrackItem items[EVENT_TRACK_BUFFER_SIZE];
    int head, tail;
    sprite_t* target_texture;
    sprite_t* item_texture;
} EventTrack;

void event_track_init(EventTrack* track, const Vec2* source_pos, const Vec2* target_pos, sprite_t* item_texture, sprite_t* target_texture, float lifetime);
void event_track_update(EventTrack* track, float deltatime);
void event_track_draw(EventTrack* track);
void event_track_reset(EventTrack* track);

/** Adds a new item onto the track.
 *  The time elapsed parameter specifies the time the item has been in flight before being added to the track. This is effectively: curent_time - (event_time - lead_time).
 */
void event_track_add_item(EventTrack* track, float time_elapsed);