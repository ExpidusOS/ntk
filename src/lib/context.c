#define G_LOG_DOMAIN "NtkContext"
#include <ntk/context.h>
#include "context-priv.h"
#include "error-priv.h"

#define NTK_CONTEXT_PRIVATE(self) ((self)->priv == NULL ? ntk_context_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkContext, ntk_context, G_TYPE_OBJECT);

enum {
	PROP_0,
	PROP_RENDERER,
	PROP_FONT_DESCRIPTION,
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

	if (priv->font_desc == NULL) {
		g_debug("Using default font");
		priv->font_desc = pango_font_description_from_string("Droid Sans Regular 12px");
	}

	g_return_if_fail(priv->renderer != NULL);
	struct nk_user_font* font = ntk_renderer_get_font(priv->renderer, priv->font_desc, &error);
	if (error != NULL) {
		g_critical("Failed to retrieve the font: %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
	} else {
		g_return_if_fail(font != NULL);

		g_debug("Initializing Nuklear");
		if (!nk_init_default(&priv->nk, font)) {
			g_critical("Failed to initialize Nuklear");
		} else {
			priv->inited = TRUE;
		}
	}
}

static void ntk_context_finalize(GObject* obj) {
	NtkContext* self = NTK_CONTEXT(obj);
	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

	if (priv->inited) {
		g_debug("Freeing Nuklear");
		nk_free(&priv->nk);
		priv->inited = FALSE;
	}

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
		case PROP_FONT_DESCRIPTION:
			priv->font_desc = g_value_get_boxed(value);
			if (priv->inited) {
				GError* error = NULL;
				struct nk_user_font* font = ntk_renderer_get_font(priv->renderer, priv->font_desc, &error);
				if (error != NULL) {
					g_critical("Failed to retrieve the font: %s:%d: %s", g_quark_to_string(error->domain), error->code, error->message);
				} else {
					g_return_if_fail(font != NULL);
					priv->nk.style.font = font;
				}
			}
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
		case PROP_FONT_DESCRIPTION:
			g_value_set_boxed(value, priv->font_desc);
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
	obj_props[PROP_FONT_DESCRIPTION] = g_param_spec_boxed("font-description", "Pango Font Description", "The description of the font to utilize.", PANGO_TYPE_FONT_DESCRIPTION, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

	obj_sigs[SIG_RENDERED] = g_signal_new("rendered", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void ntk_context_init(NtkContext* self) {
	self->priv = ntk_context_get_instance_private(self);
}

NtkContext* ntk_context_new(NtkRenderer* renderer) {
	return NTK_CONTEXT(g_object_new(NTK_TYPE_CONTEXT, "renderer", renderer, NULL));
}

gboolean ntk_context_render(NtkContext* self, NtkContextDrawCallback callback, gpointer callback_target, GError** error) {
	g_return_val_if_fail(NTK_IS_CONTEXT(self), FALSE);
	g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

	NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
	g_return_val_if_fail(priv->renderer != NULL, FALSE);

	NtkRendererType type = ntk_renderer_get_render_type(priv->renderer);
	if (type < 0) {
		ntk_error_set_bad_renderer(error, "the render type is a non-zero number", priv->renderer);
		return FALSE;
	}

  if (callback != NULL) callback(self, &priv->nk, callback_target);

	switch (type) {
		case NTK_RENDERER_TYPE_COMMAND:
			{
				NtkRendererCommand* cmd = g_malloc0(sizeof (NtkRendererCommand));
				cmd->is_vertex = 0;
				g_debug("Rendering context without vertexes");
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
				g_debug("Rendering context with vertexes");

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
	
	g_signal_emit(self, obj_sigs[SIG_RENDERED], 0);
	nk_clear(&priv->nk);
	return TRUE;
}
