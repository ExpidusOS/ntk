#ifndef __NTK_BACKEND_GTK4_WIDGET_H_
#define __NTK_BACKEND_GTK4_WIDGET_H_ 1

#include <glib-object.h>
#include <gtk/gtk.h>
#include "renderer.h"

G_BEGIN_DECLS

#define NTK_GTK4_TYPE_WIDGET ntk_gtk4_widget_get_type()
G_DECLARE_FINAL_TYPE(NtkGtk4Widget, ntk_gtk4_widget, NTK_GTK4, WIDGET, GtkWidget);

/**
 * NtkGtk4Widget:
 *
 * Since: 0.1.0
 */
struct _NtkGtk4Widget {
  GtkWidget parent_instance;

  /*< private >*/
  struct _NtkGtk4WidgetPrivate* priv;
};

GtkWidget* ntk_gtk4_widget_new();
NtkRenderer* ntk_gtk4_widget_get_renderer(NtkGtk4Widget* self);

G_END_DECLS

#endif
