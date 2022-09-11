#define G_LOG_DOMAIN "NtkRendererGtk4Widget"
#include <ntk/renderer/gtk4/widget.h>
#include "widget-priv.h"

#define NTK_RENDERER_GTK4_WIDGET_PRIVATE(self) ((self)->priv == NULL ? ntk_renderer_gtk4_widget_get_instance_private(self) : (self)->priv)

enum {
	PROP_0,
	PROP_RENDERER,
	N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

static void ntk_renderer_gtk4_widget_interface_init(GtkBuildableIface* iface);

G_DEFINE_TYPE_WITH_CODE(NtkRendererGtk4Widget, ntk_renderer_gtk4_widget, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(NtkRendererGtk4Widget)
		G_IMPLEMENT_INTERFACE(GTK_TYPE_BUILDABLE, ntk_renderer_gtk4_widget_interface_init));

static void ntk_renderer_gtk4_widget_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_renderer_gtk4_widget_parent_class)->constructed(obj);

	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	priv->renderer = ntk_renderer_gtk4_renderer_new();
}

static void ntk_renderer_gtk4_widget_finalize(GObject* obj) {
	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	g_clear_object(&priv->renderer);

	G_OBJECT_CLASS(ntk_renderer_gtk4_widget_parent_class)->finalize(obj);
}

static void ntk_renderer_gtk4_widget_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			priv->renderer = g_value_dup_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_gtk4_widget_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			g_value_set_object(value, priv->renderer);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_gtk4_widget_snapshot(GtkWidget* widget, GtkSnapshot* snapshot) {
	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(widget);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	int width = gtk_widget_get_width(widget);
	int height = gtk_widget_get_height(widget);

	ntk_renderer_request_draw(priv->renderer, width, height);

	GtkSnapshot* rendering_snapshot = NULL;
	g_object_get(priv->renderer, "snapshot", &rendering_snapshot, NULL);
	g_assert(rendering_snapshot != NULL);

	GdkPaintable* paintable = gtk_snapshot_to_paintable(rendering_snapshot, NULL);
	g_assert(paintable != NULL);
	gdk_paintable_snapshot(paintable, snapshot, width, height);
	g_object_unref(paintable);
}

static void ntk_renderer_gtk4_widget_interface_init(GtkBuildableIface* iface) {
}

static void ntk_renderer_gtk4_widget_class_init(NtkRendererGtk4WidgetClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);

	object_class->constructed = ntk_renderer_gtk4_widget_constructed;
	object_class->finalize = ntk_renderer_gtk4_widget_finalize;
	object_class->set_property = ntk_renderer_gtk4_widget_set_property;
	object_class->get_property = ntk_renderer_gtk4_widget_get_property;

	obj_props[PROP_RENDERER] = g_param_spec_object("renderer", "Ntk Renderer", "The Ntk Renderer to render with.", NTK_TYPE_RENDERER, G_PARAM_READABLE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	widget_class->snapshot = ntk_renderer_gtk4_widget_snapshot;
}

static void ntk_renderer_gtk4_widget_init(NtkRendererGtk4Widget* self) {
	self->priv = ntk_renderer_gtk4_widget_get_instance_private(self);
}

GtkWidget* ntk_renderer_gtk4_widget_new() {
	return GTK_WIDGET(g_object_new(NTK_RENDERER_GTK4_TYPE_WIDGET, NULL));
}

NtkRenderer* ntk_renderer_gtk4_widget_get_renderer(NtkRendererGtk4Widget* self) {
	g_return_val_if_fail(NTK_RENDERER_GTK4_IS_WIDGET(self), NULL);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);
	return priv->renderer;
}
