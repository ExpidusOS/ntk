#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/renderer/cairo/renderer.h>
#include <ntk/renderer/gtk4/renderer.h>

typedef struct _NtkRendererGtk4RendererPrivate {
  NtkRenderer* renderer;
} NtkRendererGtk4RendererPrivate;

#endif
