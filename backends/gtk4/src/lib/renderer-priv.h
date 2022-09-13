#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/backend/cairo/renderer.h>
#include <ntk/backend/gtk4/renderer.h>

typedef struct _NtkGtk4RendererPrivate {
  NtkRenderer* renderer;
} NtkGtk4RendererPrivate;

#endif
