#define G_LOG_DOMAIN "NtkRendererCairoRenderer"
#include <ntk/renderer/cairo/renderer.h>
#include <ntk/font.h>
#include <ntk/utils.h>
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
#include <cairo-gobject.h>
#endif
#include <math.h>
#include "renderer-priv.h"

#define NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_renderer_cairo_renderer_get_instance_private(self) : (self)->priv)

enum {
	PROP_0,
	PROP_WIDTH,
	PROP_HEIGHT,
	PROP_SURFACE,
	N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE(NtkRendererCairoRenderer, ntk_renderer_cairo_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_renderer_cairo_renderer_get_render_type(NtkRenderer* renderer) {
	return NTK_RENDERER_TYPE_COMMAND;
}

static gboolean ntk_renderer_cairo_renderer_render_command(NtkRenderer* renderer, const struct nk_command* cmd, GError** error) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(renderer);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	ntk_command_debug(cmd);

	switch (cmd->type) {
		case NK_COMMAND_RECT_FILLED:
			{
				struct nk_command_rect_filled* c = (struct nk_command_rect_filled*)cmd;
				double degrees = M_PI / 180.0;

				cairo_set_source_rgba(priv->cr, c->color.r / 255.0, c->color.g / 255.0, c->color.b / 255.0, c->color.a / 255.0);

				if (c->rounding == 0) {
					cairo_rectangle(priv->cr, c->x, c->y, c->w, c->h);
				} else {
					cairo_new_sub_path(priv->cr);
					cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->rounding, c->rounding, -90 * degrees, 0 * degrees);
					cairo_arc(priv->cr, c->x + c->w - c->rounding, c->y + c->h - c->rounding, c->rounding, 0 * degrees, 90 * degrees);
					cairo_arc(priv->cr, c->x + c->rounding, c->y + c->h - c->rounding, c->rounding, 90 * degrees, 180 * degrees);
					cairo_arc(priv->cr, c->x + c->rounding, c->y + c->rounding, c->rounding, 180 * degrees, 270 * degrees);
					cairo_close_path(priv->cr);
				}
				cairo_fill(priv->cr);
			}
			break;
	}
	return TRUE;
}

static struct nk_user_font* ntk_renderer_cairo_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(renderer);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	PangoLayout* layout = pango_layout_new(priv->pango_ctx);
	return ntk_pango_layout_font_new(layout, desc);
}

static void ntk_renderer_cairo_renderer_get_size(NtkRenderer* renderer, int* width, int* height) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(renderer);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	if (priv->width > 0 && priv->height > 0) {
		if (width) *width = priv->width;
		if (height) *height = priv->height;
	} else {
		if (width) *width = cairo_image_surface_get_width(priv->surf);
		if (height) *height = cairo_image_surface_get_height(priv->surf);
	}
}

static void ntk_renderer_cairo_renderer_set_size(NtkRenderer* renderer, int width, int height) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(renderer);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	if (priv->cr) cairo_destroy(priv->cr);

	if (priv->width > 0 && priv->height > 0) {
		priv->width = width;
		priv->height = height;

		if (priv->surf) cairo_surface_destroy(priv->surf);
		priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

		g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_WIDTH]);
		g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_HEIGHT]);
		g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
	}

	priv->cr = cairo_create(priv->surf);
}

static void ntk_renderer_cairo_renderer_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_renderer_cairo_renderer_parent_class)->constructed(obj);

	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(obj);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	priv->pango_ctx = pango_context_new();

	if (priv->width > 0 && priv->height > 0 && priv->surf == NULL) {
		priv->surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, priv->width, priv->height);
	}

	if (priv->cr == NULL) priv->cr = cairo_create(priv->surf);
}

static void ntk_renderer_cairo_renderer_finalize(GObject* obj) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(obj);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	g_clear_object(&priv->pango_ctx);
	g_clear_pointer(&priv->cr, cairo_destroy);
	g_clear_pointer(&priv->surf, cairo_surface_destroy);

	G_OBJECT_CLASS(ntk_renderer_cairo_renderer_parent_class)->finalize(obj);
}

static void ntk_renderer_cairo_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(obj);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_WIDTH:
			priv->width = g_value_get_int(value);
			ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
			break;
		case PROP_HEIGHT:
			priv->height = g_value_get_int(value);
			ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
			break;
		case PROP_SURFACE:
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
			priv->surf = g_value_get_boxed(value);
#else
			priv->surf = cairo_image_surface_reference(g_value_get_pointer(value));
#endif
			ntk_renderer_set_size(NTK_RENDERER(self), priv->width, priv->height);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_cairo_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkRendererCairoRenderer* self = NTK_RENDERER_CAIRO_RENDERER(obj);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_WIDTH:
			g_value_set_int(value, priv->width);
			break;
		case PROP_HEIGHT:
			g_value_set_int(value, priv->height);
			break;
		case PROP_SURFACE:
#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
			g_value_set_boxed(value, priv->surf);
#else
			g_value_set_pointer(value, priv->surf);
#endif
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_renderer_cairo_renderer_class_init(NtkRendererCairoRendererClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

	object_class->constructed = ntk_renderer_cairo_renderer_constructed;
	object_class->finalize = ntk_renderer_cairo_renderer_finalize;

	object_class->set_property = ntk_renderer_cairo_renderer_set_property;
	object_class->get_property = ntk_renderer_cairo_renderer_get_property;

	obj_props[PROP_WIDTH] = g_param_spec_int("width", "Width", "The width to render at.", 0, G_MAXINT, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_props[PROP_HEIGHT] = g_param_spec_int("height", "Height", "The height to render at.", 0, G_MAXINT, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

#ifdef CAIRO_HAS_GOBJECT_FUNCTIONS
	obj_props[PROP_SURFACE] = g_param_spec_boxed("surface", "Cairo Surface", "The Cairo surface to render onto", CAIRO_GOBJECT_TYPE_SURFACE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
#else
	obj_props[PROP_SURFACE] = g_param_spec_pointer("surface", "Cairo Surface", "The Cairo surface to render onto", G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
#endif

	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	renderer_class->get_render_type = ntk_renderer_cairo_renderer_get_render_type;
	renderer_class->render_command = ntk_renderer_cairo_renderer_render_command;
	renderer_class->get_font = ntk_renderer_cairo_renderer_get_font;
	renderer_class->get_size = ntk_renderer_cairo_renderer_get_size;
	renderer_class->set_size = ntk_renderer_cairo_renderer_set_size;
}

static void ntk_renderer_cairo_renderer_init(NtkRendererCairoRenderer* self) {
	self->priv = ntk_renderer_cairo_renderer_get_instance_private(self);
}

NtkRenderer* ntk_renderer_cairo_renderer_new(int width, int height) {
	return NTK_RENDERER(g_object_new(NTK_RENDERER_CAIRO_TYPE_RENDERER, "width", width, "height", height, NULL));
}

NtkRenderer* ntk_renderer_cairo_renderer_new_for_surface(cairo_surface_t* surf) {
	return NTK_RENDERER(g_object_new(NTK_RENDERER_CAIRO_TYPE_RENDERER, "surface", surf, NULL));
}

void ntk_renderer_cairo_renderer_set_surface(NtkRendererCairoRenderer* self, cairo_surface_t* value) {
	g_return_if_fail(NTK_RENDERER_CAIRO_IS_RENDERER(self));
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);

	g_clear_pointer(&priv->surf, cairo_surface_destroy);
	priv->surf = cairo_surface_reference(value);

	g_clear_pointer(&priv->cr, cairo_destroy);
	priv->cr = cairo_create(priv->surf);

	g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_SURFACE]);
}

cairo_surface_t* ntk_renderer_cairo_renderer_get_surface(NtkRendererCairoRenderer* self) {
	g_return_val_if_fail(NTK_RENDERER_CAIRO_IS_RENDERER(self), NULL);
	NtkRendererCairoRendererPrivate* priv = NTK_RENDERER_CAIRO_RENDERER_PRIVATE(self);
	return priv->surf;
}
