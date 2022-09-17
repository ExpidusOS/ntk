#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <ntk/renderer.h>

G_BEGIN_DECLS

#define NTK_GTK4_TYPE_RENDERER ntk_gtk4_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkGtk4Renderer, ntk_gtk4_renderer, NTK_GTK4, RENDERER, NtkRenderer);

struct _NtkGtk4Renderer {
  NtkRenderer parent_instance;

  /*< private >*/
  struct _NtkGtk4RendererPrivate* priv;
};

/**
 * ntk_gtk4_renderer_new: (constructor)
 */
NtkRenderer* ntk_gtk4_renderer_new();

NtkRenderer* ntk_gtk4_renderer_get_subrenderer(NtkGtk4Renderer* self);
void ntk_gtk4_renderer_snapshot_to(NtkGtk4Renderer* self, GtkSnapshot* snapshot);

G_END_DECLS
