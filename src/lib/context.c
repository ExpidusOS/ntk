#define G_LOG_DOMAIN "NtkContext"
#include <ntk/context.h>
#include "context-priv.h"
#include "error-priv.h"

#define NTK_CONTEXT_PRIVATE(self) ((self)->priv == NULL ? ntk_context_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkContext, ntk_context, G_TYPE_OBJECT);

enum {
	PROP_0,
	PROP_RENDERER,
	N_PROPERTIES
};

enum {
	SIG_RENDERED,
	N_SIGNALS
};

static GParamSpec* obj_props[N_PROPERTIES] = { NULL, };
static guint obj_sigs[N_SIGNALS] = { 0 };

static void ntk_context_constructed(GObject* obj) {
	G_OBJECT_CLASS(ntk_context_parent_class)->constructed(obj);

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

	g_clear_object(&priv->renderer);

	G_OBJECT_CLASS(ntk_context_parent_class)->finalize(obj);
}

static void ntk_context_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
	NtkContext* self = NTK_CONTEXT(obj);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			priv->renderer = g_value_dup_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_context_get_property(GObject* obj, guint prop_id, GValue* value, GParamSpec* pspec) {
	NtkContext* self = NTK_CONTEXT(obj);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

	switch (prop_id) {
		case PROP_RENDERER:
			g_value_set_object(value, priv->renderer);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void ntk_context_class_init(NtkContextClass* klass) {
	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	object_class->constructed = ntk_context_constructed;
	object_class->finalize = ntk_context_finalize;

	object_class->set_property = ntk_context_set_property;
	object_class->get_property = ntk_context_get_property;

	obj_props[PROP_RENDERER] = g_param_spec_object("renderer", "Ntk Renderer", "The Ntk Renderer to render with.", NTK_TYPE_RENDERER, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	obj_sigs[SIG_RENDERED] = g_signal_new("rendered", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, (GSignalCMarshaller)g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

static void ntk_context_init(NtkContext* self) {
	self->priv = ntk_context_get_instance_private(self);
}

NtkContext* ntk_context_new(NtkRenderer* renderer) {
	return NTK_CONTEXT(g_object_new(NTK_TYPE_CONTEXT, "renderer", renderer, NULL));
}

NtkRenderer* ntk_context_get_renderer(NtkContext* self) {
	g_return_val_if_fail(NTK_IS_CONTEXT(self), NULL);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
	return priv->renderer;
}

gboolean ntk_context_render(NtkContext* self, GError** error) {
	g_return_val_if_fail(NTK_IS_CONTEXT(self), FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
	g_return_val_if_fail(priv->renderer != NULL, FALSE);

	NtkRendererType type = ntk_renderer_get_render_type(priv->renderer);
	if (type < 0) {
		ntk_error_set_bad_renderer(error, "the render type is a non-zero number", priv->renderer);
		return FALSE;
	}

	switch (type) {
		case NTK_RENDERER_TYPE_COMMAND:
			{
				NtkRendererCommand* cmd = g_malloc0(sizeof (NtkRendererCommand));
				cmd->is_vertex = 0;
				nk_foreach(cmd->draw, &priv->nk) {
					if (!ntk_renderer_draw(priv->renderer, cmd, error)) {
						g_free(cmd);
						return FALSE;
					}
				}
				g_free(cmd);
			}
			break;
		case NTK_RENDERER_TYPE_VERTEX:
			{
				NtkRendererCommand* cmd = g_malloc0(sizeof (NtkRendererCommand));
				cmd->is_vertex = 1;

				struct nk_convert_config cfg = {};

				nk_buffer_init_default(&cmd->vertex.cmds);
				nk_buffer_init_default(&cmd->vertex.verts);
				nk_buffer_init_default(&cmd->vertex.idx);
				nk_convert(&priv->nk, &cmd->vertex.cmds, &cmd->vertex.verts, &cmd->vertex.idx, &cfg);

				nk_draw_foreach(cmd->vertex.cmd, &priv->nk, &cmd->vertex.cmds) {
					if (!ntk_renderer_draw(priv->renderer, cmd, error)) {
						return FALSE;
					}
				}

				nk_buffer_free(&cmd->vertex.cmds);
				nk_buffer_free(&cmd->vertex.verts);
				nk_buffer_free(&cmd->vertex.idx);
				g_free(cmd);
			}
			break;
	}

	nk_clear(&priv->nk);
	return TRUE;
}
