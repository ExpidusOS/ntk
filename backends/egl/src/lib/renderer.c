#define G_LOG_DOMAIN "NtkEGLRenderer"
#include <ntk/backend/egl/renderer.h>
#include <ntk/font.h>
#include "renderer-priv.h"

#define NTK_EGL_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_egl_renderer_get_instance_private(self) : (self)->priv)

enum {
	PROP_0,
	PROP_EGL_DISPLAY,
	N_PROPERTIES,
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE(NtkEGLRenderer, ntk_egl_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_egl_renderer_get_render_type(NtkRenderer* renderer) {
	(void)renderer;
	return NTK_RENDERER_TYPE_VERTEX;
}

static gboolean ntk_egl_renderer_render_vertex(NtkRenderer* renderer, NtkRendererVertexCommand* cmd, GError** error) {
	NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
	return TRUE;
}

static struct nk_user_font* ntk_egl_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
	NtkEGLRenderer* self = NTK_EGL_RENDERER(renderer);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

	PangoLayout* layout = pango_layout_new(priv->pango_ctx);
	return ntk_pango_layout_font_new(layout, desc);
}

static void ntk_egl_renderer_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_egl_renderer_parent_class)->constructed(obj);

	NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

	priv->pango_ctx = pango_context_new();
}

static void ntk_egl_renderer_finalize(GObject* obj) {
	NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

	g_clear_object(&priv->pango_ctx);

	G_OBJECT_CLASS(ntk_egl_renderer_parent_class)->finalize(obj);
}

static void ntk_egl_renderer_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_EGL_DISPLAY:
			priv->egl_display = g_value_get_pointer(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_egl_renderer_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkEGLRenderer* self = NTK_EGL_RENDERER(obj);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);

	switch (prop_id) {
		case PROP_EGL_DISPLAY:
			g_value_set_pointer(value, priv->egl_display);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_egl_renderer_class_init(NtkEGLRendererClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

	object_class->constructed = ntk_egl_renderer_constructed;
	object_class->finalize = ntk_egl_renderer_finalize;

	object_class->set_property = ntk_egl_renderer_set_property;
	object_class->get_property = ntk_egl_renderer_get_property;

	obj_props[PROP_EGL_DISPLAY] = g_param_spec_pointer("egl-display", "EGL Display", "The EGL Display Snapshot to render onto.", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	renderer_class->get_render_type = ntk_egl_renderer_get_render_type;
	renderer_class->render_vertex = ntk_egl_renderer_render_vertex;
	renderer_class->get_font = ntk_egl_renderer_get_font;
}

static void ntk_egl_renderer_init(NtkEGLRenderer* self) {
	self->priv = ntk_egl_renderer_get_instance_private(self);
}

EGLDisplay* ntk_egl_renderer_get_display(NtkEGLRenderer* self) {
	g_return_val_if_fail(NTK_EGL_IS_RENDERER(self), NULL);
	NtkEGLRendererPrivate* priv = NTK_EGL_RENDERER_PRIVATE(self);
	return priv->egl_display;
}
