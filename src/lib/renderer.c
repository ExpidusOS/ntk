#define G_LOG_DOMAIN "NtkRenderer"
#include <ntk/renderer.h>
#include "error-priv.h"

G_DEFINE_INTERFACE(NtkRenderer, ntk_renderer, G_TYPE_OBJECT);

static void ntk_renderer_default_init(NtkRendererInterface* iface) {}

/**
 * ntk_renderer_get_render_type: (virtual get_render_type)
 *
 * Returns: The type of rendering this renderer is able to do.
 */
NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self) {
	NtkRendererInterface* iface;
	g_return_val_if_fail(NTK_IS_RENDERER(self), -1);
	iface = NTK_RENDERER_GET_IFACE(self);

	g_return_val_if_fail(iface->get_render_type != NULL, -1);
	return iface->get_render_type(self);
}

/**
 * ntk_renderer_draw:
 *
 * Returns: Processes a Nuklear draw or Nuklear command.
 */
gboolean ntk_renderer_draw(NtkRenderer* self, struct nk_command* cmd, struct nk_draw_command* draw_cmd, GError** error) {
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
			return iface->render_command(self, cmd, error);
		case NTK_RENDERER_TYPE_VERTEX:
			if (iface->render_vertex == NULL) {
				ntk_error_set_bad_renderer(error, "the render type does not implement render_vertex", self);
				return FALSE;
			}
			g_return_val_if_fail(draw_cmd != NULL, FALSE);
			return iface->render_vertex(self, draw_cmd, error);
	}

	ntk_error_set_bad_renderer(error, "reached end of draw function and no action was taken", self);
	return FALSE;
}

/**
 * ntk_renderer_get_font: (virtual get_font)
 *
 * Returns: Get the Nuklear font to render with.
 */
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
