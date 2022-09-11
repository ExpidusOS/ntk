#ifndef __NTK_RENDERER_GTK4_WIDGET_H_
#define __NTK_RENDERER_GTK4_WIDGET_H_ 1

#include <glib-object.h>
#include <gtk/gtk.h>
#include "renderer.h"

G_BEGIN_DECLS

#define NTK_RENDERER_GTK4_TYPE_WIDGET ntk_renderer_gtk4_widget_get_type()
G_DECLARE_FINAL_TYPE(NtkRendererGtk4Widget, ntk_renderer_gtk4_widget, NTK_RENDERER_GTK4, WIDGET, GtkWidget);

/**
 * NtkRendererGtk4Widget:
 *
 * Since: 0.1.0
 */
struct _NtkRendererGtk4Widget {
	GtkWidget parent_instance;

	/*< private >*/
	struct _NtkRendererGtk4WidgetPrivate* priv;
};

GtkWidget* ntk_renderer_gtk4_widget_new();
NtkRenderer* ntk_renderer_gtk4_widget_get_renderer(NtkRendererGtk4Widget* self);

G_END_DECLS

#endif
