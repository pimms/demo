/* Original File taken from: https://github.com/fdeste/ADSR */

//  ADSR.c
//
//  Originally Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//  C Port 2013 Francesco D'Este

//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//

#include "adsr.h"
#include <string.h>
#include <math.h>
#include <stdio.h>


static float calcCoef(float rate, float targetRatio);
static void adsr_reset(adsr_t* ptr);

#define ADSR_DEBUG 0
#if ADSR_DEBUG == 0
    static void noop(const char * f,...) {}
    #define printf noop
    #define puts noop
#endif

void adsr_init(adsr_t* ptr)
{
    adsr_reset(ptr);
    adsr_attack_rate(ptr, 0);
    adsr_decay_rate(ptr, 0);
    adsr_release_rate(ptr, 0);
    adsr_sustain_duration(ptr, 0.1);
    adsr_sustain_level(ptr, 0.5);
    adsr_target_ratio_a(ptr,0.1);
    adsr_target_ratio_dr(ptr,0.001);
}

float adsr_process(adsr_t* ptr, double t)
{
    if (ptr->state == env_initial) {
        puts("INITIAL");
        ptr->_lastproc = t;
        ptr->_laststart = t;
        ptr->_nextstart = t;
        ptr->state = env_idle;

        adsr_offset_beats(ptr, ptr->offsetBeats);

        printf("output: %f\n", ptr->output);
        printf("attackRate: %f\n", ptr->attackRate);
        printf("decayRate: %f\n", ptr->decayRate);
        printf("releaseRate: %f\n", ptr->releaseRate);
        printf("attackCoef: %f\n", ptr->attackCoef);
        printf("decayCoef: %f\n", ptr->decayCoef);
        printf("releaseCoef: %f\n", ptr->releaseCoef);
        printf("sustainLevel: %f\n", ptr->sustainLevel);
        printf("targetRatioA: %f\n", ptr->targetRatioA);
        printf("targetRatioDR: %f\n", ptr->targetRatioDR);
        printf("attackBase: %f\n", ptr->attackBase);
        printf("decayBase: %f\n", ptr->decayBase);
        printf("releaseBase: %f\n", ptr->releaseBase);
    }

    const double dt = t - ptr->_lastproc;
	switch (ptr->state) {
        case env_idle:
            if (t < ptr->_nextstart)
                break;
            else
                /* THIS IS A FALLTHROUGH!
                 * I am not ashamed of this
                 * You can go fuck you
                 */
                ptr->state = env_attack;
            puts("->ATTACK");
        case env_attack:
            ptr->_laststart = t;
            ptr->output = ptr->attackBase + ptr->output * ptr->attackCoef;
            if (ptr->output >= 1.0) {
                ptr->output = 1.0;
                ptr->state = env_decay;
                puts("->DECAY");
            }
            break;
        case env_decay:
            ptr->output = ptr->decayBase + ptr->output * ptr->decayCoef;
            if (ptr->output <= ptr->sustainLevel) {
                ptr->output = ptr->sustainLevel;
                ptr->_susinfo.rem = ptr->_susinfo.dur;
                ptr->state = env_sustain;
                puts("->SUSTAIN");
            }
            break;
        case env_sustain:
            ptr->_susinfo.rem -= dt;
            if (ptr->_susinfo.rem <= 0.0) {
                ptr->state = env_release;
                puts("->RELEASE");
            }
            break;
        case env_release:
            ptr->output = ptr->releaseBase + ptr->output * ptr->releaseCoef;
            if (ptr->output <= 0.0) {
                ptr->output = 0.0;
                ptr->_nextstart = ptr->_laststart + (60.0 / ptr->bpm);
                ptr->state = env_idle;
                puts("->IDLE");
            }
	}

    //puts("OUT: %f\n", ptr->output);
    ptr->_lastproc = t;
	return ptr->output;
}


void adsr_bpm(adsr_t* ptr, float bpm)
{
    ptr->bpm = bpm;
}

void adsr_offset_beats(adsr_t *ptr, float offset)
{
    ptr->offsetBeats = offset;

    // This will only have an effect if we've left the initial
    // state. I.e., this function can be used to skip a beat.
    ptr->_nextstart = ptr->_laststart + (60.0 / ptr->bpm) * offset;
}

void adsr_attack_rate(adsr_t* ptr, float rate)
{
    ptr->attackRate = rate;
    ptr->attackCoef = calcCoef(rate, ptr->targetRatioA);
    ptr->attackBase = (1.0 + ptr->targetRatioA) * (1.0 - ptr->attackCoef);
}

void adsr_decay_rate(adsr_t* ptr,float rate)
{
    ptr->decayRate = rate;
    ptr->decayCoef = calcCoef(rate, ptr->targetRatioDR);
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
}

void adsr_release_rate(adsr_t* ptr, float rate)
{
    ptr->releaseRate = rate;
    ptr->releaseCoef = calcCoef(rate, ptr->targetRatioDR);
    ptr->releaseBase = -ptr->targetRatioDR * (1.0 - ptr->releaseCoef);
}

void adsr_sustain_level(adsr_t* ptr, float level)
{
    ptr->sustainLevel = level;
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
}

void adsr_sustain_duration(adsr_t *ptr, float duration)
{
    ptr->_susinfo.dur = duration;
}

void adsr_target_ratio_a(adsr_t* ptr, float targetRatio)
{
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    ptr->targetRatioA = targetRatio;
    ptr->attackBase = (1.0 + ptr->targetRatioA) * (1.0 - ptr->attackCoef);
}

void adsr_target_ratio_dr(adsr_t* ptr, float targetRatio)
{
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    ptr->targetRatioDR = targetRatio;
    ptr->decayBase = (ptr->sustainLevel - ptr->targetRatioDR) * (1.0 - ptr->decayCoef);
    ptr->releaseBase = -ptr->targetRatioDR * (1.0 - ptr->releaseCoef);
}



static float calcCoef(float rate, float targetRatio)
{
    return exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}


static void adsr_reset(adsr_t* ptr)
{
    memset(ptr, 0, sizeof(adsr_t));
    ptr->state = env_initial;
}
