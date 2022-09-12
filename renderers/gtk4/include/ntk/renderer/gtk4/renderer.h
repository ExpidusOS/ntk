#ifndef __NTK_RENDERER_GTK4_RENDERER_H_
#define __NTK_RENDERER_GTK4_RENDERER_H_ 1

#include <glib-object.h>
#include <gtk/gtk.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_RENDERER_GTK4_TYPE_RENDERER ntk_renderer_gtk4_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkRendererGtk4Renderer, ntk_renderer_gtk4_renderer, NTK_RENDERER_GTK4, RENDERER, NtkRenderer);

struct _NtkRendererGtk4Renderer {
	NtkRenderer parent_instance;

	/*< private >*/
	struct _NtkRendererGtk4RendererPrivate* priv;
};

/**
 * ntk_renderer_gtk4_renderer_new: (constructor)
 */
NtkRenderer* ntk_renderer_gtk4_renderer_new();

void ntk_renderer_gtk4_renderer_snapshot(NtkRendererGtk4Renderer* self, GtkSnapshot* snapshot);

G_END_DECLS

#endif
