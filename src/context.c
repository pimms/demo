#include "context.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


static void context_init();
static void glut_init(int argc, char **argv);
static void win_resize(GLint width, GLint height);

static void main_loop();


static void noop_func() { /* NOOP */ }
static void noop_func_update(float f) { /* NOOP */ }


struct glob_context_t
{
    context_t cur_context;
    context_t next_context;
    bool dirty_context;

    GLint width;
    GLint height;
} g_ctx;


void global_init(int argc, char **argv)
{
    context_init();
    glut_init(argc, argv);
}

void set_context(context_t context)
{
    assert(context.init_func);
    assert(context.update_func);
    assert(context.draw_func);
    assert(context.destroy_func);
    g_ctx.next_context = context;
    g_ctx.dirty_context = true;
}




static void context_init()
{
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_ctx.cur_context.init_func = noop_func;
    g_ctx.cur_context.update_func = noop_func_update;
    g_ctx.cur_context.draw_func = default_draw_func;
    g_ctx.cur_context.destroy_func = noop_func;

    g_ctx.width = 600;
    g_ctx.height = 400;
}

static void glut_init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(g_ctx.width, g_ctx.height);
    glutCreateWindow("-= demo =-");

    glutDisplayFunc(main_loop);
    glutReshapeFunc(win_resize);
    glutMainLoop();
}

static void win_resize(GLint width, GLint height)
{
    printf("resize: %dx%d\n", width, height);
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0, (GLdouble)width, 0, (GLdouble)height);

    g_ctx.width = width;
    g_ctx.height = height;
}

static void main_loop()
{
    if (g_ctx.dirty_context) {
        g_ctx.cur_context.destroy_func();
        g_ctx.next_context.init_func();
        g_ctx.cur_context = g_ctx.next_context;
        g_ctx.dirty_context = false;
    }

    g_ctx.cur_context.update_func(0.f);
    g_ctx.cur_context.draw_func();
}

