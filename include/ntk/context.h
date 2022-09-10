#ifndef __NTK_CONTEXT_H_
#define __NTK_CONTEXT_H_ 1

#include <glib-object.h>
#include "renderer.h"

G_BEGIN_DECLS

#define NTK_TYPE_CONTEXT (ntk_context_get_type())
G_DECLARE_FINAL_TYPE(NtkContext, ntk_context, NTK, CONTEXT, GObject);

struct _NtkContext {
	GObject parent_instance;
	struct _NtkContextPrivate* priv;
};

NtkContext* ntk_context_new(NtkRenderer* renderer);

G_END_DECLS

#endif
