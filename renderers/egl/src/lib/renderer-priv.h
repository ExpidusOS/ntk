#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/renderer/egl/renderer.h>

typedef struct _NtkRendererEGLRendererPrivate {
	EGLDisplay* egl_display;

	PangoContext* pango_ctx;
} NtkRendererEGLRendererPrivate;

#endif
