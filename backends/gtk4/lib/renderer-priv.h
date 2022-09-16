#pragma once

#include <ntk/backend/cairo/renderer.h>
#include <ntk/backend/gtk4/renderer.h>

typedef struct _NtkGtk4RendererPrivate {
  NtkRenderer* renderer;
} NtkGtk4RendererPrivate;
