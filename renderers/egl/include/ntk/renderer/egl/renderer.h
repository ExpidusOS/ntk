#ifndef __NTK_RENDERER_EGL_RENDERER_H_
#define __NTK_RENDERER_EGL_RENDERER_H_ 1

#include <glib-object.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_TYPE_EGL_RENDERER ntk_egl_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkRendererEGLRenderer, ntk_renderer_egl_renderer, NTK_RENDERER_EGL, RENDERER, NtkRenderer);

struct _NtkRendererEGLRenderer {
	NtkRenderer parent_instance;

	/*< private >*/
	struct _NtkRendererEGLRendererPrivate* priv;
};

/**
 * ntk_renderer_egl_renderer_new: (constructor)
 */
NtkRenderer* ntk_renderer_egl_renderer_new();

/**
 * ntk_renderer_egl_get_display:
 * @self: an #NtkRendererEGLRenderer
 * Returns: (not nullable) (transfer none): The EGL Display that is being used
 */
EGLDisplay* ntk_renderer_egl_renderer_get_display(NtkRendererEGLRenderer* self);

G_END_DECLS

#endif
