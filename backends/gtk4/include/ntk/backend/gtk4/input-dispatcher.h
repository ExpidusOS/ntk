#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <ntk/input-dispatcher.h>

G_BEGIN_DECLS

#define NTK_GTK4_TYPE_INPUT_DISPATCHER ntk_gtk4_input_dispatcher_get_type()
G_DECLARE_FINAL_TYPE(NtkGtk4InputDispatcher, ntk_gtk4_input_dispatcher, NTK_GTK4, INPUT_DISPATCHER, NtkInputDispatcher);

struct _NtkGtk4InputDispatcher {
  NtkInputDispatcher parent_instance;

  /*< private >*/
  struct _NtkGtk4InputDispatcherPrivate* priv;
};

/**
 * ntk_gtk4_input_dispatcher_new: (constructor)
 */
NtkInputDispatcher* ntk_gtk4_input_dispatcher_new(GtkWidget* widget);

/**
 * ntk_gtk4_input_dispatcher_get_widget: (get-property widget)
 */
GtkWidget* ntk_gtk4_input_dispatcher_get_widget(NtkGtk4InputDispatcher* self);

G_END_DECLS
