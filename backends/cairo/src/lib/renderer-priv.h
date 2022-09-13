#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <cairo.h>

typedef struct _NtkCairoRendererPrivate {
  PangoContext* pango_ctx;

  cairo_t* cr;
  cairo_surface_t* surf;

  int width;
  int height;
} NtkCairoRendererPrivate;

#endif
