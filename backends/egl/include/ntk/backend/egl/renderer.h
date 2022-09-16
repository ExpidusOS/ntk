#ifndef __NTK_BACKEND_EGL_RENDERER_H_
#define __NTK_BACKEND_EGL_RENDERER_H_ 1

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glib-object.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_EGL_TYPE_RENDERER ntk_egl_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkEGLRenderer, ntk_egl_renderer, NTK_EGL, RENDERER, NtkRenderer);

struct _NtkEGLRenderer {
  NtkRenderer parent_instance;

  /*< private >*/
  struct _NtkEGLRendererPrivate* priv;
};

/**
 * ntk_egl_renderer_new: (constructor)
 */
NtkRenderer* ntk_egl_renderer_new(GError** error);

/**
 * ntk_egl_get_display:
 * @self: an #NtkRendererEGLRenderer
 * Returns: (not nullable) (transfer none): The EGL Display that is being used
 */
EGLDisplay* ntk_egl_renderer_get_display(NtkEGLRenderer* self);

G_END_DECLS

#endif
