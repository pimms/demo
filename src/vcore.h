#pragma once

#include <gl.h>
#include <glut.h>

typedef void(*draw_func_t)();


void default_draw_func();
void render_clear();
void render_swap();

