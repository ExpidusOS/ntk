#define G_LOG_DOMAIN "NtkGtk4Widget"
#include <ntk/backend/gtk4/widget.h>
#include "widget-priv.h"

#define NTK_GTK4_WIDGET_PRIVATE(self) ((self)->priv == NULL ? ntk_gtk4_widget_get_instance_private(self) : (self)->priv)

enum {
	PROP_0,
	PROP_RENDERER,
	N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

static void ntk_gtk4_widget_interface_init(GtkBuildableIface* iface);

G_DEFINE_TYPE_WITH_CODE(NtkGtk4Widget, ntk_gtk4_widget, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(NtkGtk4Widget)
		G_IMPLEMENT_INTERFACE(GTK_TYPE_BUILDABLE, ntk_gtk4_widget_interface_init));

static void ntk_gtk4_widget_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_gtk4_widget_parent_class)->constructed(obj);

	NtkGtk4Widget* self = NTK_GTK4_WIDGET(obj);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);

	priv->renderer = ntk_gtk4_renderer_new();

	priv->controller_gclick = gtk_gesture_click_new();
	gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(priv->controller_gclick), 0);
	gtk_widget_add_controller(GTK_WIDGET(self), GTK_EVENT_CONTROLLER(priv->controller_gclick));

  priv->controller_key = gtk_event_controller_key_new();
	gtk_widget_add_controller(GTK_WIDGET(self), priv->controller_key);

  priv->controller_scroll = gtk_event_controller_scroll_new(GTK_EVENT_CONTROLLER_SCROLL_DISCRETE | GTK_EVENT_CONTROLLER_SCROLL_BOTH_AXES);
	gtk_widget_add_controller(GTK_WIDGET(self), priv->controller_scroll);

  priv->controller_motion = gtk_event_controller_motion_new();
	gtk_widget_add_controller(GTK_WIDGET(self), priv->controller_motion);
}

static void ntk_gtk4_widget_finalize(GObject* obj) {
	NtkGtk4Widget* self = NTK_GTK4_WIDGET(obj);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);

	if (priv->controller_gclick != NULL) {
		gtk_widget_remove_controller(GTK_WIDGET(self), GTK_EVENT_CONTROLLER(priv->controller_gclick));
		g_object_unref(priv->controller_gclick);
		priv->controller_gclick = NULL;
	}

	if (priv->controller_key != NULL) {
		gtk_widget_remove_controller(GTK_WIDGET(self), priv->controller_key);
		g_object_unref(priv->controller_key);
		priv->controller_key = NULL;
	}

	if (priv->controller_scroll != NULL) {
		gtk_widget_remove_controller(GTK_WIDGET(self), priv->controller_scroll);
		g_object_unref(priv->controller_scroll);
		priv->controller_scroll = NULL;
	}

	if (priv->controller_motion != NULL) {
		gtk_widget_remove_controller(GTK_WIDGET(self), priv->controller_motion);
		g_object_unref(priv->controller_motion);
		priv->controller_motion = NULL;
	}

	g_clear_object(&priv->renderer);

	G_OBJECT_CLASS(ntk_gtk4_widget_parent_class)->finalize(obj);
}

static void ntk_gtk4_widget_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkGtk4Widget* self = NTK_GTK4_WIDGET(obj);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			priv->renderer = g_value_dup_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_gtk4_widget_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkGtk4Widget* self = NTK_GTK4_WIDGET(obj);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			g_value_set_object(value, priv->renderer);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_gtk4_widget_compute_expand(GtkWidget* widget, gboolean* hexpand, gboolean* vexpand) {
	*hexpand = TRUE;
	*vexpand = TRUE;
}

static void ntk_gtk4_widget_snapshot(GtkWidget* widget, GtkSnapshot* snapshot) {
	NtkGtk4Widget* self = NTK_GTK4_WIDGET(widget);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);

	int width = gtk_widget_get_width(widget);
	int height = gtk_widget_get_height(widget);

	ntk_renderer_set_size(priv->renderer, width, height);
	ntk_renderer_request_draw(priv->renderer);

	NtkGtk4Renderer* renderer = NTK_GTK4_RENDERER(priv->renderer);
	ntk_gtk4_renderer_snapshot_to(renderer, snapshot);
}

static void ntk_gtk4_widget_interface_init(GtkBuildableIface* iface) {
}

static void ntk_gtk4_widget_class_init(NtkGtk4WidgetClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);

	object_class->constructed = ntk_gtk4_widget_constructed;
	object_class->finalize = ntk_gtk4_widget_finalize;
	object_class->set_property = ntk_gtk4_widget_set_property;
	object_class->get_property = ntk_gtk4_widget_get_property;

	obj_props[PROP_RENDERER] = g_param_spec_object("renderer", "Ntk Renderer", "The Ntk Renderer to render with.", NTK_TYPE_RENDERER, G_PARAM_READABLE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	widget_class->compute_expand = ntk_gtk4_widget_compute_expand;
	widget_class->snapshot = ntk_gtk4_widget_snapshot;
}

static void ntk_gtk4_widget_init(NtkGtk4Widget* self) {
	self->priv = ntk_gtk4_widget_get_instance_private(self);
}

GtkWidget* ntk_gtk4_widget_new() {
	return GTK_WIDGET(g_object_new(NTK_GTK4_TYPE_WIDGET, NULL));
}

NtkRenderer* ntk_gtk4_widget_get_renderer(NtkGtk4Widget* self) {
	g_return_val_if_fail(NTK_GTK4_IS_WIDGET(self), NULL);
	NtkGtk4WidgetPrivate* priv = NTK_GTK4_WIDGET_PRIVATE(self);
	return priv->renderer;
}
