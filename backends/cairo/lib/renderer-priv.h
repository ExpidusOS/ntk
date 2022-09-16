#pragma once

#include <cairo.h>

typedef struct _NtkCairoRendererPrivate {
  PangoContext* pango_ctx;

  cairo_t* cr;
  cairo_surface_t* surf;

  int width;
  int height;
} NtkCairoRendererPrivate;
