#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/renderer/gtk4/renderer.h>

typedef struct _NtkRendererGtk4RendererPrivate {
	GtkSnapshot* snapshot;
	GdkDrawContext* draw_context;

	PangoContext* pango_ctx;
} NtkRendererGtk4RendererPrivate;

#endif
