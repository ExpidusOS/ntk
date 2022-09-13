#define G_LOG_DOMAIN "NtkGtk4InputDispatcher"
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

static void ntk_gtk4_input_dispatcher_handle_key_pressed(GtkEventControllerKey* key, guint keyval, guint keycode, GdkModifierType state, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  if (state & GDK_SHIFT_MASK) ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_SHIFT, 1);
  if (state & GDK_CONTROL_MASK) ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_CTRL, 1);

  switch (keyval) {
    case GDK_KEY_Delete:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_DEL, 1);
      break;
    case GDK_KEY_Return:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_ENTER, 1);
      break;
    case GDK_KEY_Tab:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_TAB, 1);
      break;
    case GDK_KEY_BackSpace:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_BACKSPACE, 1);
      break;
    case GDK_KEY_Up:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_UP, 1);
      break;
    case GDK_KEY_Down:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_DOWN, 1);
      break;
    case GDK_KEY_Left:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_LEFT, 1);
      break;
    case GDK_KEY_Right:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_RIGHT, 1);
      break;
  }

  GtkIMContext* im_context = gtk_event_controller_key_get_im_context(key);
  if (im_context != NULL) gtk_im_context_focus_in(im_context);
  else ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_UNICODE, gdk_keyval_to_unicode(keyval));
}

static void ntk_gtk4_input_dispatcher_handle_key_released(GtkEventControllerKey* key, guint keyval, guint keycode, GdkModifierType state, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  if (state & GDK_SHIFT_MASK) ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_SHIFT, 0);
  if (state & GDK_CONTROL_MASK) ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_CTRL, 0);

  switch (keyval) {
    case GDK_KEY_Delete:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_DEL, 0);
      break;
    case GDK_KEY_Return:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_ENTER, 0);
      break;
    case GDK_KEY_Tab:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_TAB, 0);
      break;
    case GDK_KEY_BackSpace:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_BACKSPACE, 0);
      break;
    case GDK_KEY_Up:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_UP, 0);
      break;
    case GDK_KEY_Down:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_DOWN, 0);
      break;
    case GDK_KEY_Left:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_LEFT, 0);
      break;
    case GDK_KEY_Right:
      ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_KEY, NK_KEY_RIGHT, 0);
      break;
  }

  GtkIMContext* im_context = gtk_event_controller_key_get_im_context(key);
  if (im_context != NULL) gtk_im_context_focus_out(im_context);
}

static void ntk_gtk4_input_dispatcher_handle_im_commit(GtkIMContext* im_context, char* str, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_ASCII, str, strlen(str));
  gtk_im_context_reset(im_context);
}

static void ntk_gtk4_input_dispatcher_handle_scroll(GtkEventControllerScroll* scroll, double dx, double dy, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_SCROLL, dx, dy);
}

static void ntk_gtk4_input_dispatcher_handle_motion(GtkEventControllerMotion* motion, double dx, double dy, gpointer data) {
  NtkGtk4InputDispatcher* self = NTK_GTK4_INPUT_DISPATCHER(data);
  g_return_if_fail(NTK_GTK4_IS_INPUT_DISPATCHER(self));

  int rx = llround(dx);
  int ry = llround(dy);

  ntk_input_dispatcher_trigger(NTK_INPUT_DISPATCHER(self), NTK_INPUT_DISPATCHER_TYPE_MOTION, rx, ry);
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
#define _ATTACH_SIGNAL(obj, name, handler) sigs = g_list_append(sigs, GINT_TO_POINTER(g_signal_connect(obj, name, G_CALLBACK(handler), self)))
#define ATTACH_SIGNAL(name, handler) _ATTACH_SIGNAL(controller, name, handler)

    if (GTK_IS_GESTURE_CLICK(controller)) {
      g_debug("Attached gesture click");

      ATTACH_SIGNAL("pressed", ntk_gtk4_input_dispatcher_handle_gesture_click_pressed);
      ATTACH_SIGNAL("released", ntk_gtk4_input_dispatcher_handle_gesture_click_released);
    } else if (GTK_IS_EVENT_CONTROLLER_KEY(controller)) {
      g_debug("Attached key");

      ATTACH_SIGNAL("key-pressed", ntk_gtk4_input_dispatcher_handle_key_pressed);
      ATTACH_SIGNAL("key-released", ntk_gtk4_input_dispatcher_handle_key_released);

      GtkIMContext* im_context = gtk_event_controller_key_get_im_context(GTK_EVENT_CONTROLLER_KEY(controller));
      if (im_context != NULL) {
#define ATTACH_IM_SIGNAL(name, handler) _ATTACH_SIGNAL(im_context, name, handler)

        ATTACH_IM_SIGNAL("commit", ntk_gtk4_input_dispatcher_handle_im_commit);

#undef ATTACH_IM_SIGNAL
      }
    } else if (GTK_IS_EVENT_CONTROLLER_SCROLL(controller)) {
      g_debug("Attached scroll");

      ATTACH_SIGNAL("scroll", ntk_gtk4_input_dispatcher_handle_scroll);
    } else if (GTK_IS_EVENT_CONTROLLER_MOTION(controller)) {
      g_debug("Attached motion");

      ATTACH_SIGNAL("motion", ntk_gtk4_input_dispatcher_handle_motion);
    }

#undef ATTACH_SIGNAL
#undef _ATTACH_SIGNAL

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
