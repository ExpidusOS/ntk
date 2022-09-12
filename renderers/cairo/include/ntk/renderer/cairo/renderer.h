#ifndef __NTK_RENDERER_CAIRO_RENDERER_H_
#define __NTK_RENDERER_CAIRO_RENDERER_H_ 1

#include <glib-object.h>
#include <ntk/renderer.h>
#include <cairo.h>

G_BEGIN_DECLS

#define NTK_RENDERER_CAIRO_TYPE_RENDERER ntk_renderer_cairo_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkRendererCairoRenderer, ntk_renderer_cairo_renderer, NTK_RENDERER_CAIRO, RENDERER, NtkRenderer);

struct _NtkRendererCairoRenderer {
	NtkRenderer parent_instance;

	/*< private >*/
	struct _NtkRendererCairoRendererPrivate* priv;
};

/**
 * ntk_renderer_cairo_renderer_new: (constructor)
 */
NtkRenderer* ntk_renderer_cairo_renderer_new(int width, int height);

/**
 * ntk_renderer_cairo_renderer_new_for_surface: (constructor)
 */
NtkRenderer* ntk_renderer_cairo_renderer_new_for_surface(cairo_surface_t* surf);

/**
 * ntk_renderer_cairo_renderer_set_width: (set-property width)
 * @self: an #NtkRendererCairoRenderer
 * @value: The width to render at
 */
void ntk_renderer_cairo_renderer_set_width(NtkRendererCairoRenderer* self, int value);

/**
 * ntk_renderer_cairo_renderer_get_width: (get-property width)
 * @self: an #NtkRendererCairoRenderer
 * Returns: The height the renderer is outputting at
 */
int ntk_renderer_cairo_renderer_get_width(NtkRendererCairoRenderer* self);

/**
 * ntk_renderer_cairo_renderer_set_height: (set-property height)
 * @self: an #NtkRendererCairoRenderer
 * @value: The width to render at
 */
void ntk_renderer_cairo_renderer_set_height(NtkRendererCairoRenderer* self, int value);

/**
 * ntk_renderer_cairo_renderer_get_height: (get-property height)
 * @self: an #NtkRendererCairoRenderer
 * Returns: The height the renderer is outputting at
 */
int ntk_renderer_cairo_renderer_get_height(NtkRendererCairoRenderer* self);

/**
 * ntk_renderer_cairo_renderer_set_surface: (set-property surface)
 * @self: an #NtkRendererCairoRenderer
 * @value: The surface
 */
void ntk_renderer_cairo_renderer_set_surface(NtkRendererCairoRenderer* self, cairo_surface_t* value);

/**
 * ntk_renderer_cairo_renderer_get_surface: (get-property surface)
 * @self: an #NtkRendererCairoRenderer
 * Returns: (not nullable) (transfer none): The Cairo Surface being used.
 */
cairo_surface_t* ntk_renderer_cairo_renderer_get_surface(NtkRendererCairoRenderer* self);

G_END_DECLS

#endif
