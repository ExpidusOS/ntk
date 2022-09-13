#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/backend/egl/renderer.h>

typedef struct _NtkEGLRendererPrivate {
	EGLDisplay* egl_display;

	PangoContext* pango_ctx;
} NtkEGLRendererPrivate;

#endif
