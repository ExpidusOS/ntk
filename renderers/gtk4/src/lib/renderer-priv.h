#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/renderer/gtk4/renderer.h>

typedef struct _NtkRendererGtk4RendererPrivate {
	PangoContext* pango_ctx;
	int width;
	int height;

	cairo_surface_t* surf;
	cairo_t* cr;
} NtkRendererGtk4RendererPrivate;

#endif
