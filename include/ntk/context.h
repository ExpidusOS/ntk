#ifndef __NTK_CONTEXT_H_
#define __NTK_CONTEXT_H_ 1

#include <glib-object.h>
#include "renderer.h"

G_BEGIN_DECLS

#define NTK_TYPE_CONTEXT (ntk_context_get_type())
G_DECLARE_FINAL_TYPE(NtkContext, ntk_context, NTK, CONTEXT, GObject);

/**
 * NtkContext:
 *
 * Since: 0.1.0
 */
struct _NtkContext {
	GObject parent_instance;

	/*< private >*/
	struct _NtkContextPrivate* priv;
};

typedef void (*NtkContextDrawCallback)(NtkContext* context, struct nk_context* nuklear, gpointer user_data);

/**
 * ntk_context_new: (construct)
 */
NtkContext* ntk_context_new(NtkRenderer* renderer);

gboolean ntk_context_render(NtkContext* self, NtkContextDrawCallback callback, gpointer callback_target, GError** error);

G_END_DECLS

#endif
