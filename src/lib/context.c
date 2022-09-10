#define G_LOG_DOMAIN "NtkContext"
#include <ntk/context.h>
#include "context-priv.h"

#define NTK_CONTEXT_PRIVATE(self) ((self)->priv == NULL ? ntk_context_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkContext, ntk_context, G_TYPE_OBJECT);

static void ntk_context_constructed(GObject* obj) {
	G_OBJECT_CLASS(obj)->constructed(obj);

	NtkContext* self = NTK_CONTEXT(obj);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
	GError* error = NULL;

	g_return_if_fail(priv->renderer != NULL);
	struct nk_user_font* font = ntk_renderer_get_font(priv->renderer, NULL, &error); // TODO: find the default font we want to use
	if (error != NULL) {
		g_critical("Failed to retrieve the font: %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
	} else {
		g_return_if_fail(font != NULL);

		g_debug("Initializing Nuklear");
		if (!nk_init_default(&priv->nk, font)) {
			g_critical("Failed to initialize Nuklear");
		}
	}
}

static void ntk_context_finalize(GObject* obj) {
	NtkContext* self = NTK_CONTEXT(obj);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

	g_debug("Freeing Nuklear");
	nk_free(&priv->nk);

	G_OBJECT_CLASS(obj)->finalize(obj);
}

static void ntk_context_class_init(NtkContextClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	object_class->constructed = ntk_context_constructed;
	object_class->finalize = ntk_context_finalize;
}

static void ntk_context_init(NtkContext* self) {
	self->priv = ntk_context_get_instance_private(self);
}

NtkContext* ntk_context_new(NtkRenderer* renderer) {
	return NTK_CONTEXT(g_object_new(NTK_TYPE_CONTEXT, NULL));
}
