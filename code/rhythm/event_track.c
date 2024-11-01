#include "event_track.h"

#include <math.h>


void event_track_init(EventTrack* track, const Vec2* source_pos, const Vec2* target_pos, sprite_t* item_texture, float lifetime) {
    track->source_pos = *source_pos;
    track->target_pos = *target_pos;
    track->item_texture = item_texture;

    // calclulate the normalized direction vector that all items in this track will move on.
    track->direction.x = target_pos->x - source_pos->x;
    track->direction.y = target_pos->y - source_pos->y;
    const float distance = sqrtf(track->direction.x * track->direction.x + track->direction.y * track->direction.y);
    track->direction.x /= distance;
    track->direction.y /= distance;

    // constant speed all items on this track will travel
    track->speed = distance / lifetime;

    event_track_reset(track);
}

void event_track_update(EventTrack* track, float deltatime) {
    const Vec2 step = {track->direction.x * track->speed * deltatime, track->direction.y * track->speed * deltatime};
    int current = track->head;

    // TODO: prune items that have gone off the screen?
    while (current != track->tail) {
        EventTrackItem* current_item = &track->items[current];
        current_item->position.x += step.x;
        current_item->position.y += step.y;

        current += 1;
        if (current == EVENT_TRACK_BUFFER_SIZE) {
            current = 0;
        }
    }
}

void event_track_draw(EventTrack* track) {
    int current = track->head;
    while (current != track->tail) {
        EventTrackItem* current_item = &track->items[current];

        rdpq_sprite_blit(track->item_texture, 
            current_item->position.x - track->item_texture->width / 2, 
            current_item->position.y - track->item_texture->height / 2 , 
            NULL);

        current += 1;
        if (current == EVENT_TRACK_BUFFER_SIZE) {
            current = 0;
        }
    }
}

void event_track_reset(EventTrack* track) {
    track->head = 0;
    track->tail = 0;
}

void event_track_add_item(EventTrack* track, float time_elapsed) {
    EventTrackItem* item = &track->items[track->tail];
    
    // The current playback time will need to be taken into account when determining the staritng position if this method is called
    // after the actual time for the event to be added has passed.
    // Track speed may be set up to deliver items from start to end in 0.3 seconds but we may be notified of the event with
    // only 2.95 seconds before it is due to reach it's destination.
    const Vec2 step = {track->direction.x * track->speed * time_elapsed, track->direction.y * track->speed * time_elapsed};
    item->position.x = track->source_pos.x + step.x;
    item->position.y = track->source_pos.y + step.y;

    track->tail += 1;
    if (track->tail == EVENT_TRACK_BUFFER_SIZE) {
        track->tail = 0;
    }
}
