#pragma once

#include <stdint.h>

#include "adsr.h"

#define AUDIO_BITRATE (44100)

typedef float(*audio_func_t)(double t);



float default_audio_func(double t);


