#ifndef __parallax_h
#define __parallax_h
#include <lcthw/darray.h>
#include "../prefix.h"
#include "../math/vpoint.h"
#include "camera.h"
#include "graphics.h"

typedef struct ParallaxLayer {
    GfxTexture *texture;
    VPoint offset;
    double scale;
    double p_factor;
} ParallaxLayer;

typedef struct Parallax {
    DArray *layers;
    Camera *camera;
    GfxSize level_size;
    GfxUVertex sky_color;
    GfxUVertex earth_color;
} Parallax;

Parallax *Parallax_create(GfxSize level_size, Camera *camera);
void Parallax_destroy(Parallax *parallax);
int Parallax_add_layer(Parallax *parallax, GfxTexture *texture, VPoint offset,
        double scale, double p_factor);
void Parallax_render(Parallax *parallax, Graphics *graphics);

#endif