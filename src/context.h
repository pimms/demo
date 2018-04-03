#pragma once

#include "vrender.h"

typedef void(*context_init_func_t)();
typedef void(*context_update_func_t)(float);
typedef void(*context_destroy_func_t)();

typedef struct context_t {
    context_init_func_t init_func;
    context_update_func_t update_func;
    draw_func_t draw_func;
    context_destroy_func_t destroy_func;
} context_t;

void global_init(int argc, char **argv);
void set_context(context_t context);

