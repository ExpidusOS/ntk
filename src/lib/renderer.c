#define G_LOG_DOMAIN "NtkRenderer"
#include <ntk/renderer.h>
#include "error-priv.h"
#include "renderer-priv.h"

#define NTK_RENDERER_PRIVATE(self) (ntk_renderer_get_instance_private(self))

G_DEFINE_TYPE_WITH_PRIVATE(NtkRenderer, ntk_renderer, G_TYPE_OBJECT);

enum {
	SIG_REQUEST_DRAW,
	SIG_DRAW,
	N_SIGNALS
};

static guint obj_sigs[N_SIGNALS] = { 0 };

static void ntk_renderer_class_init(NtkRendererClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	obj_sigs[SIG_REQUEST_DRAW] = g_signal_new("request-draw", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
	obj_sigs[SIG_DRAW] = g_signal_new("draw", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static void ntk_renderer_init(NtkRenderer* self) {
}

void ntk_renderer_request_draw(NtkRenderer* self) {
	g_return_if_fail(NTK_IS_RENDERER(self));
	g_signal_emit(self, obj_sigs[SIG_REQUEST_DRAW], 0);
}

void ntk_renderer_set_size(NtkRenderer* self, int width, int height) {
	NtkRendererClass* klass;
	g_return_if_fail(NTK_IS_RENDERER(self));
	klass = NTK_RENDERER_GET_CLASS(self);

	if (klass->set_size != NULL) {
		klass->set_size(self, width, height);
	} else {
		NtkRendererPrivate* priv = NTK_RENDERER_PRIVATE(self);

		priv->width = width;
		priv->height = height;
	}
}

void ntk_renderer_get_size(NtkRenderer* self, int* width, int* height) {
	NtkRendererClass* klass;
	g_return_if_fail(NTK_IS_RENDERER(self));
	klass = NTK_RENDERER_GET_CLASS(self);

	if (klass->get_size != NULL) {
		int tmp = 0;
		if (!width) width = &tmp;
		if (!height) height = &tmp;
		klass->get_size(self, width, height);
	} else {
		NtkRendererPrivate* priv = NTK_RENDERER_PRIVATE(self);

		if (width) *width = priv->width;
		if (height) *height = priv->height;
	}
}

NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self) {
	NtkRendererClass* klass;
	g_return_val_if_fail(NTK_IS_RENDERER(self), -1);
	klass = NTK_RENDERER_GET_CLASS(self);

	g_return_val_if_fail(klass->get_render_type != NULL, -1);
	return klass->get_render_type(self);
}

gboolean ntk_renderer_draw(NtkRenderer* self, NtkRendererCommand* cmd, GError** error) {
	NtkRendererClass* klass;
	g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
	klass = NTK_RENDERER_GET_CLASS(self);

	NtkRendererType type = ntk_renderer_get_render_type(self);
	if (type < 0) {
		ntk_error_set_bad_renderer(error, "the render type is a non-zero number", self);
		return FALSE;
	}

	gboolean result = FALSE;

	switch (type) {
		case NTK_RENDERER_TYPE_COMMAND:
			if (klass->render_command == NULL) {
				ntk_error_set_bad_renderer(error, "the render type does not implement render_command", self);
				return FALSE;
			}

			g_return_val_if_fail(cmd != NULL, FALSE);
			g_return_val_if_fail(!cmd->is_vertex, FALSE);
			result = klass->render_command(self, cmd->draw, error);
			g_signal_emit(self, obj_sigs[SIG_DRAW], 0, cmd);
			return result;
		case NTK_RENDERER_TYPE_VERTEX:
			if (klass->render_vertex == NULL) {
				ntk_error_set_bad_renderer(error, "the render type does not implement render_vertex", self);
				return FALSE;
			}

			g_return_val_if_fail(cmd != NULL, FALSE);
			g_return_val_if_fail(cmd->is_vertex, FALSE);
			result = klass->render_vertex(self, &cmd->vertex, error);
			g_signal_emit(self, obj_sigs[SIG_DRAW], 0, cmd);
			return result;
	}

	ntk_error_set_bad_renderer(error, "reached end of draw function and no action was taken", self);
	return FALSE;
}

struct nk_user_font* ntk_renderer_get_font(NtkRenderer* self, PangoFontDescription* desc, GError** error) {
	NtkRendererClass* klass;
	g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
	g_return_val_if_fail(desc != NULL, FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
	klass = NTK_RENDERER_GET_CLASS(self);

	if (klass->get_font == NULL) {
		ntk_error_set_bad_renderer(error, "the render type does not implement get_font", self);
		return FALSE;
	}
	return klass->get_font(self, desc, error);
}
