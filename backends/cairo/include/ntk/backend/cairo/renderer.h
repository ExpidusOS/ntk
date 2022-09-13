#ifndef __NTK_BACKEND_CAIRO_RENDERER_H_
#define __NTK_BACKEND_CAIRO_RENDERER_H_ 1

#include <glib-object.h>
#include <ntk/renderer.h>
#include <cairo.h>

G_BEGIN_DECLS

#define NTK_CAIRO_TYPE_RENDERER ntk_cairo_renderer_get_type()
G_DECLARE_FINAL_TYPE(NtkCairoRenderer, ntk_cairo_renderer, NTK_CAIRO, RENDERER, NtkRenderer);

struct _NtkCairoRenderer {
	NtkRenderer parent_instance;

	/*< private >*/
	struct _NtkCairoRendererPrivate* priv;
};

/**
 * ntk_cairo_renderer_new: (constructor)
 */
NtkRenderer* ntk_cairo_renderer_new(int width, int height);

/**
 * ntk_cairo_renderer_new_for_surface: (constructor)
 */
NtkRenderer* ntk_cairo_renderer_new_for_surface(cairo_surface_t* surf);

/**
 * ntk_cairo_renderer_set_width: (set-property width)
 * @self: an #NtkCairoRenderer
 * @value: The width to render at
 */
void ntk_cairo_renderer_set_width(NtkCairoRenderer* self, int value);

/**
 * ntk_cairo_renderer_get_width: (get-property width)
 * @self: an #NtkCairoRenderer
 * Returns: The height the renderer is outputting at
 */
int ntk_cairo_renderer_get_width(NtkCairoRenderer* self);

/**
 * ntk_cairo_renderer_set_height: (set-property height)
 * @self: an #NtkCairoRenderer
 * @value: The width to render at
 */
void ntk_cairo_renderer_set_height(NtkCairoRenderer* self, int value);

/**
 * ntk_cairo_renderer_get_height: (get-property height)
 * @self: an #NtkCairoRenderer
 * Returns: The height the renderer is outputting at
 */
int ntk_cairo_renderer_get_height(NtkCairoRenderer* self);

/**
 * ntk_cairo_renderer_set_surface: (set-property surface)
 * @self: an #NtkCairoRenderer
 * @value: The surface
 */
void ntk_cairo_renderer_set_surface(NtkCairoRenderer* self, cairo_surface_t* value);

/**
 * ntk_cairo_renderer_get_surface: (get-property surface)
 * @self: an #NtkCairoRenderer
 * Returns: (not nullable) (transfer none): The Cairo Surface being used.
 */
cairo_surface_t* ntk_cairo_renderer_get_surface(NtkCairoRenderer* self);

G_END_DECLS

#endif
