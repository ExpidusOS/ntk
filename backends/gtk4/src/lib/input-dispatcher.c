#define G_LOG_DOMAN "NtkGtk4InputDispatcher"
#include <ntk/backend/gtk4/input-dispatcher.h>
#include "input-dispatcher-priv.h"
#include <math.h>

#define NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self) ((self)->priv == NULL ? ntk_gtk4_input_dispatcher_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkGtk4InputDispatcher, ntk_gtk4_input_dispatcher, NTK_TYPE_INPUT_DISPATCHER);

enum {
  PROP_0,
  PROP_WIDGET,
  N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };
static guint nuklear_gtk4_button_map[] = {
  [GDK_BUTTON_PRIMARY] = NK_BUTTON_LEFT,
  [GDK_BUTTON_MIDDLE] = NK_BUTTON_MIDDLE,
  [GDK_BUTTON_SECONDARY] = NK_BUTTON_RIGHT
};

static void ntk_gtk4_input_dispatcher_handle_gesture_click_pressed(GtkGestureClick* gclick, int n_pressed, double x, double y, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  int rx = llround(x);
  int ry = llround(y);

  guint btn;
  g_object_get(gclick, "button", &btn, NULL);

  if (n_pressed < 2) {
    ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_BUTTON, nuklear_gtk4_button_map[btn], rx, ry, 1);
  } else if (btn == GDK_BUTTON_PRIMARY) {
    ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_BUTTON, NK_BUTTON_DOUBLE, rx, ry, 1);
  }
}

static void ntk_gtk4_input_dispatcher_handle_gesture_click_released(GtkGestureClick* gclick, int n_pressed, double x, double y, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  int rx = llround(x);
  int ry = llround(y);

  guint btn;
  g_object_get(gclick, "button", &btn, NULL);

  if (n_pressed < 2) {
    ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_BUTTON, nuklear_gtk4_button_map[btn], rx, ry, 0);
  } else if (btn == GDK_BUTTON_PRIMARY) {
    ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_BUTTON, NK_BUTTON_DOUBLE, rx, ry, 0);
  }
}

static void ntk_gtk4_input_dispatcher_connect_controllers(NtkGtk4InputDispatcher* self) {
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);

  g_assert(priv->controllers != NULL);

  for (guint i = 0; i < g_list_model_get_n_items(priv->controllers); i++) {
    GtkEventController* controller = GTK_EVENT_CONTROLLER(g_list_model_get_item(priv->controllers, i));

    const gchar* name = gtk_event_controller_get_name(controller);
    if (name == NULL) {
      gtk_event_controller_set_name(controller, g_strdup_printf("controller-%d", i));
      name = gtk_event_controller_get_name(controller);
    }
    g_assert(name != NULL);

    if (g_hash_table_contains(priv->handlers, name)) continue;

    GList* sigs = NULL;
#define ATTACH_SIGNAL(controller, name, handler) sigs = g_list_append(sigs, GINT_TO_POINTER(g_signal_connect(controller, name, G_CALLBACK(handler), self)))

    if (GTK_IS_GESTURE_CLICK(controller)) {
      ATTACH_SIGNAL(GTK_GESTURE_CLICK(controller), "pressed", ntk_gtk4_input_dispatcher_handle_gesture_click_pressed);
      ATTACH_SIGNAL(GTK_GESTURE_CLICK(controller), "released", ntk_gtk4_input_dispatcher_handle_gesture_click_released);
    }

#undef ATTACH_SIGNAL

    if (sigs != NULL) g_hash_table_insert(priv->handlers, g_strdup(name), sigs);
  }

  GHashTableIter iter;
  g_hash_table_iter_init(&iter, priv->handlers);
  gpointer key;
  gpointer value;
  while (g_hash_table_iter_next(&iter, &key, &value)) {
    gchar* iter_name = (gchar*)key;
    for (guint i = 0; i < g_list_model_get_n_items(priv->controllers); i++) {
      GtkEventController* controller = GTK_EVENT_CONTROLLER(g_list_model_get_item(priv->controllers, i));

      const gchar* name = gtk_event_controller_get_name(controller);
      if (name == NULL) {
        gtk_event_controller_set_name(controller, g_strdup_printf("controller-%d", i));
        name = gtk_event_controller_get_name(controller);
      }
      g_assert(name != NULL);

      if (!g_strcmp0(name, iter_name)) continue;

      g_hash_table_iter_remove(&iter);
      break;
    }
  }
}

static void ntk_gtk4_input_dispatcher_controllers_change(GListModel* model, guint pos, guint removed, guint added, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  ntk_gtk4_input_dispatcher_connect_controllers(self);
}

static void ntk_gtk4_input_dispatcher_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_gtk4_input_dispatcher_parent_class)->constructed(obj);

  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(obj);
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);

  priv->handlers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)g_list_free);

  priv->controllers = gtk_widget_observe_controllers(priv->widget);
  g_assert(priv->controllers != NULL);

  priv->controllers_change_id = g_signal_connect(priv->controllers, "items-changed", G_CALLBACK(ntk_gtk4_input_dispatcher_controllers_change), self);
  ntk_gtk4_input_dispatcher_connect_controllers(self);
}

static void ntk_gtk4_input_dispatcher_finalize(GObject* obj) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(obj);
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);

  g_clear_signal_handler(&priv->controllers_change_id, priv->controllers);
  g_clear_pointer(&priv->handlers, g_hash_table_destroy);
  g_clear_object(&priv->controllers);
  g_clear_object(&priv->widget);

  G_OBJECT_CLASS(ntk_gtk4_input_dispatcher_parent_class)->finalize(obj);
}

static void ntk_gtk4_input_dispatcher_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(obj);
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);

  switch (prop_id) {
    case PROP_WIDGET:
      priv->widget = g_value_dup_object(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_gtk4_input_dispatcher_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(obj);
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);

  switch (prop_id) {
    case PROP_WIDGET:
      g_value_set_object(value, priv->widget);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void ntk_gtk4_input_dispatcher_class_init(NtkGtk4InputDispatcherClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = ntk_gtk4_input_dispatcher_constructed;
  object_class->finalize = ntk_gtk4_input_dispatcher_finalize;

  object_class->set_property = ntk_gtk4_input_dispatcher_set_property;
  object_class->get_property = ntk_gtk4_input_dispatcher_get_property;

  obj_props[PROP_WIDGET] = g_param_spec_object("widget", "Gtk Widget", "The Gtk widget to pull events from", GTK_TYPE_WIDGET, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);
}

static void ntk_gtk4_input_dispatcher_init(NtkGtk4InputDispatcher* self) {
  self->priv = ntk_gtk4_input_dispatcher_get_instance_private(self);
}

NtkInputDispatcher* ntk_gtk4_input_dispatcher_new(GtkWidget* widget) {
  return NTK_INPUT_DISPATCHER(g_object_new(NTK_GTK4_TYPE_INPUT_DISPATCHER, "widget", widget, NULL));
}

GtkWidget* ntk_gtk4_input_dispatcher_get_widget(NtkGtk4InputDispatcher* self) {
  g_return_val_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self), NULL);
  NtkGtk4InputDispatcherPrivate* priv = NTK_GTK4_INPUT_DISPATCHER_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);
  return priv->widget;
}
