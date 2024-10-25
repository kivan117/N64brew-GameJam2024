#include "track.h"

void track_init(Track* track, int mixer_channel, const char* audio_file_asset, const char* simfile_asset, float event_lifetime) {
    track->mixer_channel = mixer_channel;
    simfile_init(&track->simfile);

    wav64_open(&track->audio_file, audio_file_asset);
    simfile_open(&track->simfile, simfile_asset);
    simfile_playback_init(&track->playback, &track->simfile, event_lifetime);
    debugf("track_init: %s\n", audio_file_asset);
}

void track_unload(Track* track) {
    mixer_ch_stop(track->mixer_channel);
    wav64_close(&track->audio_file);
    simfile_uninit(&track->simfile);
    debugf("track unload\n");
}

void track_update(Track* track, float time) {
    simfile_playback_update(&track->playback, time);

    if (!simfile_playback_finished(&track->playback) && !mixer_ch_playing(track->mixer_channel)) {
        debugf("simfile update: %f\n", track->playback.current_time);
    }

    // TODO: do we need to do some sort of sync here? if playbacktime is greader than 0?
    if (!simfile_playback_finished(&track->playback) && track->playback.current_time >= 0 && !mixer_ch_playing(track->mixer_channel)) {
        debugf("start playing track!!\n");
        wav64_play(&track->audio_file, track->mixer_channel);
    }
}

void track_reset(Track* track) {
    mixer_ch_stop(track->mixer_channel);
    simfile_playback_reset(&track->playback);
}
