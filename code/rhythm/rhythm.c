#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include <string.h>

const MinigameDef minigame_def = {
    .gamename = "Rhythm Game",
    .developername = "Your Name",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};

wav64_t junkie_loop;
sprite_t* a_button;
sprite_t* b_button;
sprite_t* indicator;

typedef struct {
    float cx, cy;
    int indicator_active;
    sprite_t* button;
} Note;

#define NOTE_COUNT 8
static Note notes[NOTE_COUNT];


static void note_init(Note* note, float cx, float cy, sprite_t* button) {
    note->cx = cx;
    note->cy = cy;
    note->indicator_active = 0;
    note->button = button;
}

#define GAME_BACKGROUND     0x000000FF

static float note_times[NOTE_COUNT] = {
    0.0f, 0.3f, 0.57f,
    1.103f, 1.38f, 1.657f,
    2.188, 2.460
};

static const float indicator_time = 0.3f;
static const float song_offset_time = -0.3f;

float song_time = song_offset_time;
size_t active_note = 0;

static void draw_notes() {
    rdpq_blitparms_t params;
    memset(&params, 0, sizeof(rdpq_blitparms_t));

    for (size_t i = 0; i < NOTE_COUNT; i++) {
        Note* note = &notes[i];

        if (note->indicator_active) {
            float time_remaining_to_note = note_times[active_note] - song_time;           
            float indicator_scale = 0.5f + ((time_remaining_to_note / indicator_time)* 0.5f) ;

            float indicator_x = note->cx - (indicator->width / 2) * indicator_scale;
            float indicator_y = note->cy - (indicator->height / 2) * indicator_scale;

            params.scale_x = indicator_scale;
            params.scale_y = indicator_scale;

            rdpq_sprite_blit(indicator, indicator_x, indicator_y, &params);
        }

        rdpq_sprite_blit(note->button, note->cx - note->button->width / 2, note->cy - note->button->height /2 , NULL);
    }
}

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    wav64_open(&junkie_loop, "rom:/rhythm/junkie_loop.wav64");
    a_button = sprite_load("rom:/core/AButton.sprite");
    b_button = sprite_load("rom:/core/BButton.sprite");
    indicator = sprite_load("rom:/rhythm/indicator.sprite");
    //wav64_play(&junkie_loop, 1);


    note_init(&notes[0], 40.0f, 40.0f, a_button);
    note_init(&notes[1], 80.0f, 40.0f, a_button);
    note_init(&notes[2], 120.0f, 40.0f, b_button);

    note_init(&notes[3], 60.0f, 80.0f, a_button);
    note_init(&notes[4], 100.0f, 80.0f, a_button);
    note_init(&notes[5], 140.0f, 80.0f, b_button);

    note_init(&notes[6], 80.0f, 120.0f, a_button);
    note_init(&notes[7], 120.0f, 120.0f, b_button);


}

/*==============================
    minigame_fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is 
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
void minigame_fixedloop(float deltatime)
{
    song_time += deltatime;

    // do we need to show the indicator for the next note?
    if (song_time + indicator_time >= note_times[active_note]) {
        if (!mixer_ch_playing(1)) {
            wav64_play(&junkie_loop, 1);
        }

        notes[active_note].indicator_active = 1;
    }

    // did we reach the note?
    if (song_time >= note_times[active_note]) {
        notes[active_note].indicator_active = 0;
        active_note += 1;

        // reached end of the loop
        if (active_note >= NOTE_COUNT) {
            active_note = 0;
            song_time = song_offset_time;
            mixer_ch_stop(1);
        }
    }
}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), NULL);
    rdpq_clear(color_from_packed32(GAME_BACKGROUND));

    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1);
    
    draw_notes();

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    wav64_close(&junkie_loop);
    sprite_free(a_button);
    sprite_free(b_button);
}