#define G_LOG_DOMAIN "NtkRendererGtk4"
#include <ntk/renderer/gtk4/renderer.h>
#include <ntk/font.h>
#include "renderer-priv.h"

#define NTK_RENDERER_GTK4_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_renderer_gtk4_renderer_get_instance_private(self) : (self)->priv)

enum {
	PROP_0,
	PROP_SNAPSHOT,
	PROP_DRAW_CONTEXT,
	N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE(NtkRendererGtk4Renderer, ntk_renderer_gtk4_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_renderer_gtk4_renderer_get_render_type(NtkRenderer* renderer) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	if (priv->draw_context == NULL) return NTK_RENDERER_TYPE_COMMAND;
	if (GDK_IS_CAIRO_CONTEXT(priv->draw_context)) return NTK_RENDERER_TYPE_COMMAND;
	return NTK_RENDERER_TYPE_VERTEX;
}

static gboolean ntk_renderer_gtk4_renderer_render_command(NtkRenderer* renderer, const struct nk_command* cmd, GError** error) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);
	return TRUE;
}

static gboolean ntk_renderer_gtk4_renderer_render_vertex(NtkRenderer* renderer, NtkRendererVertexCommand* cmd, GError** error) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	if (priv->draw_context == NULL) {
	}
	return TRUE;
}

static struct nk_user_font* ntk_renderer_gtk4_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	PangoLayout* layout = pango_layout_new(priv->pango_ctx);
	return ntk_pango_layout_font_new(layout, desc);
}

static void ntk_renderer_gtk4_renderer_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_renderer_gtk4_renderer_parent_class)->constructed(obj);

	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	priv->pango_ctx = pango_context_new();

	if (priv->snapshot == NULL) {
		g_debug("Creating a new GTK Snapshot for %p", self);

		priv->snapshot = gtk_snapshot_new();

		GdkRGBA red, green, yellow, blue;
		gdk_rgba_parse(&red, "red");
		gdk_rgba_parse(&green, "green");
		gdk_rgba_parse(&yellow, "yellow");
		gdk_rgba_parse(&blue, "blue");

		float w = 100;
		float h = 100;

		gtk_snapshot_append_color(priv->snapshot, &red, &GRAPHENE_RECT_INIT(0, 0, w, h));
  	gtk_snapshot_append_color(priv->snapshot, &green, &GRAPHENE_RECT_INIT(w, 0, w, h));
  	gtk_snapshot_append_color(priv->snapshot, &yellow, &GRAPHENE_RECT_INIT(0, h, w, h));
  	gtk_snapshot_append_color(priv->snapshot, &blue, &GRAPHENE_RECT_INIT(w, h, w, h));
	}
}

static void ntk_renderer_gtk4_renderer_finalize(GObject* obj) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	g_clear_object(&priv->pango_ctx);
	g_clear_object(&priv->snapshot);

	G_OBJECT_CLASS(ntk_renderer_gtk4_renderer_parent_class)->finalize(obj);
}

static void ntk_renderer_gtk4_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_SNAPSHOT:
			priv->snapshot = g_value_dup_object(value);
			break;
		case PROP_DRAW_CONTEXT:
			priv->draw_context = g_value_dup_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_gtk4_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_SNAPSHOT:
			g_value_set_object(value, priv->snapshot);
			break;
		case PROP_DRAW_CONTEXT:
			g_value_set_object(value, priv->draw_context);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_gtk4_renderer_class_init(NtkRendererGtk4RendererClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

	object_class->constructed = ntk_renderer_gtk4_renderer_constructed;
	object_class->finalize = ntk_renderer_gtk4_renderer_finalize;

	object_class->set_property = ntk_renderer_gtk4_renderer_set_property;
	object_class->get_property = ntk_renderer_gtk4_renderer_get_property;

	obj_props[PROP_SNAPSHOT] = g_param_spec_object("snapshot", "Gtk4 Snapshot", "The Gtk4 Snapshot to render onto.", GTK_TYPE_SNAPSHOT, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	obj_props[PROP_DRAW_CONTEXT] = g_param_spec_object("draw-context", "Gdk Draw Context", "The (optional) Gdk drawing context to utilize with the snapshot.", GDK_TYPE_DRAW_CONTEXT, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	renderer_class->get_render_type = ntk_renderer_gtk4_renderer_get_render_type;
	renderer_class->render_command = ntk_renderer_gtk4_renderer_render_command;
	renderer_class->render_vertex = ntk_renderer_gtk4_renderer_render_vertex;
	renderer_class->get_font = ntk_renderer_gtk4_renderer_get_font;
}

static void ntk_renderer_gtk4_renderer_init(NtkRendererGtk4Renderer* self) {
	self->priv = ntk_renderer_gtk4_renderer_get_instance_private(self);
}

NtkRenderer* ntk_renderer_gtk4_renderer_new() {
	return NTK_RENDERER(g_object_new(NTK_RENDERER_GTK4_TYPE_RENDERER, NULL));
}

NtkRendererGtk4Renderer* ntk_renderer_gtk4_renderer_new_with_snapshot(GtkSnapshot* snapshot, GdkDrawContext* draw_context) {
	return NTK_RENDERER_GTK4_RENDERER(g_object_new(NTK_RENDERER_GTK4_TYPE_RENDERER, "snapshot", snapshot, "draw-context", draw_context, NULL));
}

GtkSnapshot* ntk_renderer_gtk4_renderer_get_snapshot(NtkRendererGtk4Renderer* self) {
	g_return_val_if_fail(NTK_RENDERER_GTK4_IS_RENDERER(self), NULL);
	NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);
	return priv->snapshot;
}
