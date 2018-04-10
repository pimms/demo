#include "acore.h"

#include <vec.h>
#include <stdlib.h>
#include <stdio.h>

inline static float randf()
{
    return (float)rand()/(float)(RAND_MAX/1.f);
}


static const float BPM = 120.f;
static const float BEAT_DUR = 60.f / BPM;

adsr_t kick;
adsr_t snare;
int init = 0;

double prevt;
double beatacc = 0.0;
int beat;


float default_audio_func(double t)
{
    if (init == 0) {
        adsr_init(&kick);
        adsr_bpm(&kick, BPM);
        adsr_attack_rate(&kick, 99);
        adsr_decay_rate(&kick, 5000);
        adsr_sustain_level(&kick, 0.05);
        adsr_sustain_duration(&kick, 0.2f);

        adsr_init(&snare);
        adsr_bpm(&snare, BPM);
        adsr_attack_rate(&snare, 99);
        adsr_decay_rate(&snare, 5000);
        adsr_sustain_level(&snare, 0.05);
        adsr_offset_beats(&snare, 0.5f);

        init = 1;
        prevt = t;
    } else {
        beatacc += t - prevt;
        prevt = t;
        if (beatacc > BEAT_DUR) {
            beatacc -= BEAT_DUR;
            beat = (beat + 1) % 4;
        }
    }

    float val = 0.f;

    // Kick
    val += sin(60 * t) * adsr_process(&kick, t);

    // Snare
    float whiteNoise = randf() * 0.4f;
    val += whiteNoise * adsr_process(&snare, t) + sin(90*t);


    double freq = 0.0;
    switch (beat) {
        case 0:
        case 2:
            freq = 1500;
            break;
        case 1:
            freq = 1750;
            break;
        case 3:
            freq = 2000;
            break;
    }
    //val += sin(freq*t);


    return val;
}
