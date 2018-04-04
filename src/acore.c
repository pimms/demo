#include "acore.h"

#include <math.h>
#include <stdlib.h>

inline float randf()
{
    return (float)rand()/(float)(RAND_MAX/1.f);
}


adsr_t kick;
adsr_t snare;
int init = 0;

float default_audio_func(double t)
{
    if (init == 0) {
        adsr_init(&kick);
        adsr_bpm(&kick, 120.f);
        adsr_attack_rate(&kick, 100);
        adsr_decay_rate(&kick, 3000);
        adsr_sustain_level(&kick, 0.3);
        adsr_sustain_duration(&kick, 0.1);

        adsr_init(&snare);
        adsr_bpm(&snare, 120.f);
        adsr_attack_rate(&snare, 99);
        adsr_decay_rate(&snare, 5000);
        adsr_sustain_level(&snare, 0.05);
        adsr_offset_beats(&snare, 0.5f);

        init = 1;
    }

    float val = 0.f;

    // Kick
    val += sin(30 * t) * adsr_process(&kick, t);

    // Snare
    float whiteNoise = randf() * 0.4;
    val += whiteNoise * adsr_process(&snare, t) + sin(90*t);


    return val;
}
