#include "track.h"

#include <assert.h>

void track_init(Track* track, int mixer_channel) {
    track->is_loaded = 0;
    track->mixer_channel = mixer_channel;
    simfile_init(&track->simfile);
}

void track_load(Track* track, const char* audio_file_asset, const char* simfile_asset, float event_lifetime) {
    track_unload(track);

    wav64_open(&track->audio_file, audio_file_asset);
    assert(simfile_open(&track->simfile, simfile_asset));
    simfile_playback_init(&track->playback, &track->simfile, event_lifetime);
}

void track_unload(Track* track) {
    if (!track->is_loaded) {
        return;
    }

    mixer_ch_stop(track->mixer_channel);
    wav64_close(&track->audio_file);
    simfile_uninit(&track->simfile);
}

void track_update(Track* track, float time) {
    simfile_playback_update(&track->playback, time);

    if (!simfile_playback_finished(&track->playback) && track->playback.current_time > 0 && !mixer_ch_playing(track->mixer_channel)) {
        wav64_play(&track->audio_file, track->mixer_channel);
    }
}

void track_reset(Track* track) {
    mixer_ch_stop(track->mixer_channel);
    simfile_playback_reset(&track->playback);
}
