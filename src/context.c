#include "context.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include <portaudio.h>


static void demo_init(int argc, char **argv);
static void demo_destroy();
static void context_init();
static void glut_init(int argc, char **argv);
static void audio_init();
static void audio_destroy();
static void audio_error(PaError err);

static void win_resize(GLint width, GLint height);
static void main_loop_cb();
static int main_audio_cb(const void*, void *,
                         unsigned long, const PaStreamCallbackTimeInfo*,
                         PaStreamCallbackFlags, void*);



static void noop_func() { /* NOOP */ }
static void noop_func_update(float f) { /* NOOP */ }


struct demo_context_t
{
    context_t cur_context;
    context_t next_context;
    bool dirty_context;

    GLint width;
    GLint height;

    PaStream *paStream;
} g_ctx;



void demo_run(int argc, char **argv)
{
    demo_init(argc, argv);
    glutMainLoop();
    demo_destroy();
}

void set_context(context_t context)
{
    assert(context.init_func);
    assert(context.update_func);
    assert(context.draw_func);
    assert(context.destroy_func);

    if (!context.audio_func) {
        context.audio_func = g_ctx.cur_context.audio_func;
    }

    g_ctx.next_context = context;
    g_ctx.dirty_context = true;
}



static void demo_init(int argc, char **argv)
{
    context_init();
    glut_init(argc, argv);
    audio_init();
}

static void demo_destroy()
{
    audio_destroy();
}

static void context_init()
{
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_ctx.cur_context.init_func = noop_func;
    g_ctx.cur_context.update_func = noop_func_update;
    g_ctx.cur_context.draw_func = default_draw_func;
    g_ctx.cur_context.audio_func = default_audio_func;
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

    glutDisplayFunc(main_loop_cb);
    glutReshapeFunc(win_resize);
}

static void audio_init()
{
    PaError err = Pa_Initialize();
    if (err != paNoError) audio_error(err);

    err = Pa_OpenDefaultStream(&g_ctx.paStream,
                               0,
                               1,
                               paFloat32,
                               AUDIO_BITRATE,
                               paFramesPerBufferUnspecified,
                               main_audio_cb,
                               NULL);
    if (err != paNoError) audio_error(err);

    err = Pa_StartStream(g_ctx.paStream);
    if (err != paNoError) audio_error(err);
}

static void audio_destroy()
{
    PaError err;

    err = Pa_StopStream(g_ctx.paStream);
    if (err != paNoError) audio_error(err);

    err = Pa_Terminate();
    if (err != paNoError) audio_error(err);
}

static void audio_error(PaError err)
{
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    exit(1);
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

static void main_loop_cb()
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

static int main_audio_cb(const void *input,
                         void *output,
                         unsigned long frames,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags flags,
                         void *userData)
{
    float *out = (float*)output;

    double curTime = timeInfo->outputBufferDacTime;
    const double delta = 1.0 / (double)AUDIO_BITRATE;
    for (uint32_t i=0; i<frames; i++ )
    {
        *out++ = g_ctx.cur_context.audio_func(curTime);
        curTime += delta;
    }

    return 0;
}
