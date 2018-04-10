#pragma once

#include "vec.h"

typedef struct
{
    int xbound;
    int ybound;
    int res;
    vec_t *vertices;
} heightmap_t;

heightmap_t* heightmap_create();
void heightmap_free(heightmap_t *heightmap);


/**
 * A function 'z = f(x, y)'. Passed as a parameter to 'heightmap_init_xy'
 * to initialize the heightmap.
 */
typedef float(*heightmap_init_coord_z_func_t)(float x, float y);

/**
 * Initialize a heightmap with a 'z = f(x, y)' function (parameter 'func').
 *
 * @param hm        The heightmap_t to initialize. If 'vertices' is non-NULL,
 *                  it will be safely freed for you.
 * @param xdim      Units long in the X-direction.
 * @param xdim      Units long in the Y-direction.
 * @param res       The number of subdivisions along the X and Y axes.
 * @param func      Function returning the Z-value at the given (x,y)
 *                  coordinate. This function will be called 'res * res' times.
 */
void heightmap_init_coord_z(heightmap_t *hm, int xdim, int ydim, int res,
                            heightmap_init_coord_z_func_t func);
