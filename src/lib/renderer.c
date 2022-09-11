#define G_LOG_DOMAIN "NtkRenderer"
#include <ntk/renderer.h>
#include "error-priv.h"

G_DEFINE_INTERFACE(NtkRenderer, ntk_renderer, G_TYPE_OBJECT);

static void ntk_renderer_default_init(NtkRendererInterface* iface) {}

NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self) {
	NtkRendererInterface* iface;
	g_return_val_if_fail(NTK_IS_RENDERER(self), -1);
	iface = NTK_RENDERER_GET_IFACE(self);

	g_return_val_if_fail(iface->get_render_type != NULL, -1);
	return iface->get_render_type(self);
}

gboolean ntk_renderer_draw(NtkRenderer* self, NtkRendererCommand* cmd, GError** error) {
	NtkRendererInterface* iface;
	g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
	iface = NTK_RENDERER_GET_IFACE(self);

	NtkRendererType type = ntk_renderer_get_render_type(self);
	if (type < 0) {
		ntk_error_set_bad_renderer(error, "the render type is a non-zero number", self);
		return FALSE;
	}

	switch (type) {
		case NTK_RENDERER_TYPE_COMMAND:
			if (iface->render_command == NULL) {
				ntk_error_set_bad_renderer(error, "the render type does not implement render_command", self);
				return FALSE;
			}

			g_return_val_if_fail(cmd != NULL, FALSE);
			g_return_val_if_fail(cmd->is_vertex, FALSE);
			return iface->render_command(self, cmd->draw, error);
		case NTK_RENDERER_TYPE_VERTEX:
			if (iface->render_vertex == NULL) {
				ntk_error_set_bad_renderer(error, "the render type does not implement render_vertex", self);
				return FALSE;
			}

			g_return_val_if_fail(cmd != NULL, FALSE);
			g_return_val_if_fail(!cmd->is_vertex, FALSE);
			return iface->render_vertex(self, &cmd->vertex, error);
	}

	ntk_error_set_bad_renderer(error, "reached end of draw function and no action was taken", self);
	return FALSE;
}

struct nk_user_font* ntk_renderer_get_font(NtkRenderer* self, PangoFontDescription* desc, GError** error) {
	NtkRendererInterface* iface;
	g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
	g_return_val_if_fail(desc != NULL, FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
	iface = NTK_RENDERER_GET_IFACE(self);

	if (iface->get_font == NULL) {
		ntk_error_set_bad_renderer(error, "the render type does not implement get_font", self);
		return FALSE;
	}
	return iface->get_font(self, desc, error);
}
