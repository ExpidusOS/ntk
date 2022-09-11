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

G_DEFINE_TYPE_WITH_PRIVATE(NtkRendererGtk4Widget, ntk_renderer_gtk4_widget, GTK_TYPE_WIDGET);

static void ntk_renderer_gtk4_widget_constructed(GObject* obj) {
	G_OBJECT_CLASS(obj)->constructed(obj);

	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	priv->renderer = ntk_renderer_gtk4_renderer_new();
}

static void ntk_renderer_gtk4_widget_finalize(GObject* obj) {
	NtkRendererGtk4Widget* self = NTK_RENDERER_GTK4_WIDGET(obj);
	NtkRendererGtk4WidgetPrivate* priv = NTK_RENDERER_GTK4_WIDGET_PRIVATE(self);

	g_clear_object(&priv->renderer);

	G_OBJECT_CLASS(obj)->finalize(obj);
}

static void ntk_renderer_gtk4_widget_class_init(NtkRendererGtk4WidgetClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	object_class->constructed = ntk_renderer_gtk4_widget_constructed;
	object_class->finalize = ntk_renderer_gtk4_widget_finalize;
}

static void ntk_renderer_gtk4_widget_init(NtkRendererGtk4Widget* self) {
	self->priv = ntk_renderer_gtk4_widget_get_instance_private(self);
}

GtkWidget* ntk_renderer_gtk4_widget_new() {
	return GTK_WIDGET(g_object_new(NTK_RENDERER_GTK4_TYPE_WIDGET, NULL));
}
