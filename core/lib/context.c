#define G_LOG_DOMAIN "NtkContext"
#include "context-priv.h"
#include "error-priv.h"
#include <gio/gio.h>
#include <ntk/context.h>

#define NTK_CONTEXT_PRIVATE(self) ((self)->priv == NULL ? ntk_context_get_instance_private(self) : (self)->priv)

static void ntk_context_interface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkContext, ntk_context, G_TYPE_OBJECT,
  G_ADD_PRIVATE(NtkContext) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_context_interface_init)
);

enum {
  PROP_0,
  PROP_RENDERER,
  PROP_FONT,
  N_PROPERTIES
};

enum {
  SIG_RENDERED,
  N_SIGNALS
};

static GParamSpec* obj_props[N_PROPERTIES] = {
  NULL,
};
static guint obj_sigs[N_SIGNALS] = {0};

static void ntk_context_finalize(GObject* obj) {
  NtkContext* self = NTK_CONTEXT(obj);
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

  if (priv->inited) {
    g_debug("Freeing Nuklear");
    nk_free(&priv->nk);
    priv->inited = FALSE;
  }

  g_clear_pointer(((struct nk_user_font**)&priv->nk.style.font), ntk_user_font_free);
  g_clear_object(&priv->renderer);
  g_clear_object(&priv->font);

  G_OBJECT_CLASS(ntk_context_parent_class)->finalize(obj);
}

static void ntk_context_set_property(GObject* obj, guint prop_id, const GValue* value, GParamSpec* pspec) {
  NtkContext* self = NTK_CONTEXT(obj);
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

  switch (prop_id) {
    case PROP_RENDERER:
      priv->renderer = g_value_dup_object(value);
      break;
    case PROP_FONT:
      priv->font = g_value_get_object(value);
      if (priv->inited) {
        g_clear_pointer(&priv->nk.style.font, ntk_user_font_free);
        priv->nk.style.font = (struct nk_user_font*)ntk_font_get_handle(priv->font);
        g_assert(priv->nk.style.font != NULL);
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
    case PROP_FONT:
      g_value_set_object(value, priv->font);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static gboolean ntk_context_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  NtkContext* self = NTK_CONTEXT(initable);
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);

  g_return_val_if_fail(priv->renderer != NULL, FALSE);
  g_return_val_if_fail(priv->font != NULL, FALSE);
  struct nk_user_font* font = (struct nk_user_font*)ntk_font_get_handle(priv->font);
  g_return_val_if_fail(font != NULL, FALSE);

  g_debug("Initializing Nuklear");
  if (!nk_init_default(&priv->nk, font)) {
    ntk_error_set_nuklear_fail(error, "Nuklear failed to initalize", self);
    return FALSE;
  }

  priv->inited = TRUE;
  return TRUE;
}

static void ntk_context_interface_init(GInitableIface* iface) {
  iface->init = ntk_context_initable_init;
}

static void ntk_context_class_init(NtkContextClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = ntk_context_finalize;

  object_class->set_property = ntk_context_set_property;
  object_class->get_property = ntk_context_get_property;

  /**
   * NtkContext:renderer: (skip)
   *
   * FIXME: figure out why the getter is invalid according to "g-ir-compiler"
   */
  obj_props[PROP_RENDERER] = g_param_spec_object(
    "renderer", "Ntk Renderer", "The Ntk Renderer to render with.", NTK_TYPE_RENDERER,
    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
  );

  /**
   * NtkContext:font-description: (type PangoFontDescription)
   */
  obj_props[PROP_FONT] = g_param_spec_object(
    "font", "Ntk Font", "The font to use.", NTK_TYPE_FONT,
    G_PARAM_CONSTRUCT | G_PARAM_READWRITE
  );
  g_object_class_install_properties(object_class, N_PROPERTIES, obj_props);

  obj_sigs[SIG_RENDERED] =
    g_signal_new("rendered", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void ntk_context_init(NtkContext* self) {
  self->priv = ntk_context_get_instance_private(self);
}

NtkContext* ntk_context_new(NtkRenderer* renderer, NtkFont* font, GError** error) {
  return g_initable_new(NTK_TYPE_CONTEXT, NULL, error, "renderer", renderer, "font", font, NULL);
}

NtkRenderer* ntk_context_get_renderer(NtkContext* self) {
  g_return_val_if_fail(NTK_IS_CONTEXT(self), NULL);
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);
  return priv->renderer;
}

NtkFont* ntk_context_get_font(NtkContext* self) {
  g_return_val_if_fail(NTK_IS_CONTEXT(self), NULL);
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
  g_return_val_if_fail(priv != NULL, NULL);
  return g_object_ref(priv->font);
}

void ntk_context_set_font(NtkContext* self, NtkFont* value) {
  g_return_if_fail(NTK_IS_CONTEXT(self));
  NtkContextPrivate* priv = NTK_CONTEXT_PRIVATE(self);
  g_return_if_fail(priv != NULL);

  priv->font = g_object_ref(value);
  g_object_notify_by_pspec(G_OBJECT(self), obj_props[PROP_FONT]);
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

  if (priv->is_drawing) return TRUE;
  priv->is_drawing = TRUE;

  if (callback != NULL) callback(self, &priv->nk, callback_target);

  switch (type) {
    case NTK_RENDERER_TYPE_COMMAND:
      {
        NtkRendererCommand* cmd = g_malloc0(sizeof(NtkRendererCommand));
        cmd->is_vertex = 0;
        g_debug("Rendering context without vertexes");
        nk_foreach (cmd->draw, &priv->nk) {
          if (!ntk_renderer_draw(priv->renderer, cmd, error)) {
            priv->is_drawing = FALSE;
            g_free(cmd);
            return FALSE;
          }
        }
        g_free(cmd);
      }
      break;
    case NTK_RENDERER_TYPE_VERTEX:
      {
        NtkRendererCommand* cmd = g_malloc0(sizeof(NtkRendererCommand));
        cmd->is_vertex = 1;
        g_debug("Rendering context with vertexes");

        struct nk_convert_config cfg = {};
        if (!ntk_renderer_configure_vertex(priv->renderer, &cfg, error)) {
          priv->is_drawing = FALSE;
          g_free(cmd);
          return FALSE;
        }

        nk_buffer_init_default(&cmd->vertex.cmds);
        nk_buffer_init_default(&cmd->vertex.verts);
        nk_buffer_init_default(&cmd->vertex.idx);
        nk_convert(&priv->nk, &cmd->vertex.cmds, &cmd->vertex.verts, &cmd->vertex.idx, &cfg);

        nk_draw_foreach (cmd->vertex.cmd, &priv->nk, &cmd->vertex.cmds) {
          if (!ntk_renderer_draw(priv->renderer, cmd, error)) {
            priv->is_drawing = FALSE;
            g_free(cmd);
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

  ntk_renderer_request_draw(priv->renderer);
  priv->is_drawing = FALSE;
  return TRUE;
}
