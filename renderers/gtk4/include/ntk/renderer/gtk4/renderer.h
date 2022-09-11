#ifndef __NTK_RENDERER_GTK4_RENDERER_H_
#define __NTK_RENDERER_GTK4_RENDERER_H_ 1

#include <glib-object.h>
#include <gtk/gtk.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_RENDERER_GTK4_TYPE_RENDERER ntk_renderer_gtk4_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkRendererGtk4Renderer, ntk_renderer_gtk4_renderer, NTK_RENDERER_GTK4, RENDERER, GObject);

struct _NtkRendererGtk4Renderer {
	GObject parent_instance;

	/*< private >*/
	struct _NtkRendererGtk4RendererPrivate* priv;
};

/**
 * ntk_renderer_gtk4_renderer_new: (constructor)
 */
NtkRenderer* ntk_renderer_gtk4_renderer_new();

/**
 * ntk_renderer_gtk4_renderer_new_with_snapshot: (constructor)
 * @snapshot: (nullable): The GTK Snapshot to render onto.
 * @draw_context: (nullable): The Gdk draw context to render with.
 */
NtkRendererGtk4Renderer* ntk_renderer_gtk4_renderer_new_with_snapshot(GtkSnapshot* snapshot, GdkDrawContext* draw_context);

/**
 * ntk_renderer_gtk4_renderer_get_snapshot: (get-property snapshot)
 * @self: an #NtkRendererGtk4Renderer
 * Returns: (not nullable) (transfer none): The snapshot used to render onto.
 */
GtkSnapshot* ntk_renderer_gtk4_renderer_get_snapshot(NtkRendererGtk4Renderer* self);

G_END_DECLS

#endif
