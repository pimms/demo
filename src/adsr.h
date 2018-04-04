/* Original File taken from: https://github.com/fdeste/ADSR */

//  ADSR.h
//
//  Originally Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//  C Port by Francesco D'Este
//
//  For a complete explanation of the adsr_t envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//

#pragma once

enum envState
{
    env_initial = 0,
    env_idle,
    env_attack,
    env_decay,
    env_sustain,
    env_release
};

typedef struct
{
    int state;
	float output;
	float attackRate;
	float decayRate;
	float releaseRate;
	float attackCoef;
	float decayCoef;
	float releaseCoef;
	float sustainLevel;
    float targetRatioA;
    float targetRatioDR;
    float attackBase;
    float decayBase;
    float releaseBase;

    struct
    {
        double dur;
        double rem;
    } _susinfo;

    float offsetBeats;
    float bpm;
    double _lastproc;
    double _laststart;
    double _nextstart;
} adsr_t;

void adsr_init(adsr_t *adsr);

float adsr_process(adsr_t* ptr, double t);

void adsr_bpm(adsr_t* ptr, float bpm);
void adsr_offset_beats(adsr_t* ptr, float offset);
void adsr_attack_rate(adsr_t* ptr, float rate);
void adsr_decay_rate(adsr_t* ptr, float rate);
void adsr_release_rate(adsr_t* ptr, float rate);
void adsr_sustain_level(adsr_t* ptr, float level);
void adsr_sustain_duration(adsr_t *ptr, float length);
void adsr_target_ratio_a(adsr_t* ptr, float targetRatio);
void adsr_target_ratio_dr(adsr_t* ptr, float targetRatio);

