#pragma once

#include <glib-object.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_GLES2_TYPE_RENDERER ntk_gles2_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkGLES2Renderer, ntk_gles2_renderer, NTK_GLES2, RENDERER, NtkRenderer);

struct _NtkGLES2Renderer {
  NtkRenderer parent_instance;

  /*< private >*/
  struct _NtkGLES2RendererPrivate* priv;
};

typedef void* (*NtkGLES2RendererLoadProcHandler)(const char* name);

/**
 * ntk_gles2_renderer_new: (constructor)
 */
NtkRenderer* ntk_gles2_renderer_new(NtkGLES2RendererLoadProcHandler proc_handler, GError** error);

G_END_DECLS
