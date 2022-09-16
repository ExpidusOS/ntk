#ifndef __NTK_CONTEXT_H_
#define __NTK_CONTEXT_H_ 1

#include "renderer.h"
#include <glib-object.h>
#include <ntk/font.h>

G_BEGIN_DECLS

#define NTK_TYPE_CONTEXT (ntk_context_get_type())
G_DECLARE_FINAL_TYPE(NtkContext, ntk_context, NTK, CONTEXT, GObject);

/**
 * NtkContext:
 *
 * The Ntk context is used to create a new Nuklear instance.
 * The events and rendering gets connected to this.
 *
 * Since: 0.1.0
 */
struct _NtkContext {
  GObject parent_instance;

  /*< private >*/
  struct _NtkContextPrivate* priv;
};

/**
 * NtkContextDrawCallback:
 * @context: The #NtkContext begin rendered
 * @nuklear: The Nuklear context, required for Nuklear functions
 * @user_data: (nullable): The user data supplied
 *
 * The callback type to handle drawing the UI elements in Nuklear
 *
 * Since: 0.1.0
 */
typedef void (*NtkContextDrawCallback)(NtkContext* context, struct nk_context* nuklear, gpointer user_data);

/**
 * ntk_context_new: (constructor)
 * @renderer: The renderer to use to render.
 * @font: The font to use.
 * @error: (out): The pointer to store the error.
 * Returns: An instance of #NtkContext
 */
NtkContext* ntk_context_new(NtkRenderer* renderer, NtkFont* font, GError** error);

/**
 * ntk_context_get_renderer: (skip)
 * @self: an #NtkContext
 * Returns: Get the renderer which is used for rendering.
 */
NtkRenderer* ntk_context_get_renderer(NtkContext* self);

/**
 * ntk_context_get_font: (get-property font)
 * @self: an #NtkContext
 * Returns: (transfer full): The font being used.
 */
NtkFont* ntk_context_get_font(NtkContext* self);

/**
 * ntk_context_set_font: (set-property font)
 * @self: an #NtkContext
 * @value: (transfer full): The value to set the font to.
 */
void ntk_context_set_font(NtkContext* self, NtkFont* value);

/**
 * ntk_context_render:
 * @self: an #NtkContext
 * @callback: (nullable): The callback to execute for rendering Nuklear widgets.
 * @callback_target: (nullable): The user data to provide to the callback.
 * @error: (out): The pointer to store the error.
 *
 * Renders a frame of the UI and process any events which were dispatched by #NtkInputDispatcher.
 * Depending on the renderer type, this may draw using vertecies or with Nuklear commands.
 *
 * Returns: TRUE if the rendering worked, FALSE if #GError was set.
 */
gboolean ntk_context_render(NtkContext* self, NtkContextDrawCallback callback, gpointer callback_target, GError** error);

G_END_DECLS

#endif
