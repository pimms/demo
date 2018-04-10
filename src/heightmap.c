#include "heightmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


static void heightmap_free_verts(heightmap_t *hm);


heightmap_t *heightmap_create()
{
    heightmap_t *hm = malloc(sizeof(heightmap_t));
    memset(hm, 0, sizeof(heightmap_t));
    return hm;
}

void heightmap_free(heightmap_t *hm)
{
    heightmap_free_verts(hm);
    free(hm);
}

static void heightmap_free_verts(heightmap_t *hm)
{
    if (hm->vertices) {
        free(hm->vertices);
        hm->vertices = NULL;
        hm->xbound = 0;
        hm->ybound = 0;
        hm->res = 0;
    }
}

void heightmap_init_xy(heightmap_t *hm, int xdim, int ydim, int res,
                       heightmap_init_coord_z_func_t func)
{
    assert(hm);
    assert(xdim > 0);
    assert(ydim > 0);
    assert(res > 0);
    assert(func);

    heightmap_free_verts(hm);

    hm->xbound = xdim;
    hm->ybound = ydim;
    hm->res = res;
    hm->vertices = malloc(res * res * sizeof(vec_t));

    const float xorigin = -xdim / 2.f;
    const float yorigin = -ydim / 2.f;

    for (int iy=0; iy<res; iy++) {
        for (int ix=0; ix<res; ix++) {
            const float xcoord = xorigin + ix*res;
            const float ycoord = yorigin + iy*res;
            const float zcoord = func(xcoord, ycoord);

            const int index = iy*res + ix;
            hm->vertices[index].x = xcoord;
            hm->vertices[index].y = ycoord;
            hm->vertices[index].z = zcoord;
        }
    }
}



