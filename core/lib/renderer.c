#include "error-priv.h"
#include "renderer-priv.h"
#include <ntk/renderer.h>

#define NTK_RENDERER_PRIVATE(self) (ntk_renderer_get_instance_private(self))

G_DEFINE_TYPE_WITH_PRIVATE(NtkRenderer, ntk_renderer, G_TYPE_OBJECT);

enum {
  SIG_REQUEST_DRAW,
  SIG_RENDERED,
  N_SIGNALS
};

static guint obj_sigs[N_SIGNALS] = {0};

static void ntk_renderer_class_init(NtkRendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  obj_sigs[SIG_REQUEST_DRAW] =
    g_signal_new("request-draw", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
  obj_sigs[SIG_RENDERED] = g_signal_new(
    "rendered", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__POINTER,
    G_TYPE_NONE, 1, G_TYPE_POINTER
  );
}

static void ntk_renderer_init(NtkRenderer* self) {}

void ntk_renderer_request_draw(NtkRenderer* self) {
  g_return_if_fail(NTK_IS_RENDERER(self));
  g_signal_emit(self, obj_sigs[SIG_REQUEST_DRAW], 0);
}

void ntk_renderer_set_size(NtkRenderer* self, int width, int height) {
  NtkRendererClass* klass;
  g_return_if_fail(NTK_IS_RENDERER(self));
  klass = NTK_RENDERER_GET_CLASS(self);

  if (klass->set_size != NULL) {
    klass->set_size(self, width, height);
  } else {
    NtkRendererPrivate* priv = NTK_RENDERER_PRIVATE(self);

    priv->width = width;
    priv->height = height;
  }
}

void ntk_renderer_get_size(NtkRenderer* self, int* width, int* height) {
  NtkRendererClass* klass;
  g_return_if_fail(NTK_IS_RENDERER(self));
  klass = NTK_RENDERER_GET_CLASS(self);

  if (klass->get_size != NULL) {
    int tmp = 0;
    if (!width) width = &tmp;
    if (!height) height = &tmp;
    klass->get_size(self, width, height);
  } else {
    NtkRendererPrivate* priv = NTK_RENDERER_PRIVATE(self);

    if (width) *width = priv->width;
    if (height) *height = priv->height;
  }
}

NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self) {
  NtkRendererClass* klass;
  g_return_val_if_fail(NTK_IS_RENDERER(self), -1);
  klass = NTK_RENDERER_GET_CLASS(self);

  g_return_val_if_fail(klass->get_render_type != NULL, -1);
  return klass->get_render_type(self);
}

gboolean ntk_renderer_draw(NtkRenderer* self, NtkRendererCommand* cmd, GError** error) {
  NtkRendererClass* klass;
  g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  klass = NTK_RENDERER_GET_CLASS(self);

  NtkRendererType type = ntk_renderer_get_render_type(self);
  if (type < 0) {
    ntk_error_set_bad_renderer(error, "the render type is a non-zero number", self);
    return FALSE;
  }

  gboolean result = FALSE;

  switch (type) {
    case NTK_RENDERER_TYPE_COMMAND:
      if (klass->render_command == NULL) {
        ntk_error_set_bad_renderer(error, "the render type does not implement render_command", self);
        return FALSE;
      }

      g_return_val_if_fail(cmd != NULL, FALSE);
      g_return_val_if_fail(!cmd->is_vertex, FALSE);
      result = klass->render_command(self, cmd->draw, error);
      g_signal_emit(self, obj_sigs[SIG_RENDERED], 0, cmd);
      return result;
    case NTK_RENDERER_TYPE_VERTEX:
      if (klass->render_vertex == NULL) {
        ntk_error_set_bad_renderer(error, "the render type does not implement render_vertex", self);
        return FALSE;
      }

      g_return_val_if_fail(cmd != NULL, FALSE);
      g_return_val_if_fail(cmd->is_vertex, FALSE);
      result = klass->render_vertex(self, &cmd->vertex, error);
      g_signal_emit(self, obj_sigs[SIG_RENDERED], 0, cmd);
      return result;
  }

  ntk_error_set_bad_renderer(error, "reached end of draw function and no action was taken", self);
  return FALSE;
}

gboolean ntk_renderer_configure_vertex(NtkRenderer* self, struct nk_convert_config* cfg, GError** error) {
  NtkRendererClass* klass;
  g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  klass = NTK_RENDERER_GET_CLASS(self);

  NtkRendererType type = ntk_renderer_get_render_type(self);
  if (type < 0) {
    ntk_error_set_bad_renderer(error, "the render type is a non-zero number", self);
    return FALSE;
  }

  g_return_val_if_fail(type == NTK_RENDERER_TYPE_VERTEX, FALSE);

  if (klass->configure_vertex == NULL) {
    ntk_error_set_bad_renderer(error, "the render type does not implement configure_vertex", self);
    return FALSE;
  }

  return klass->configure_vertex(self, cfg, error);
}

NtkFont* ntk_renderer_get_font(NtkRenderer* self, gchar* name, int size, GError** error) {
  NtkRendererClass* klass;
  g_return_val_if_fail(NTK_IS_RENDERER(self), FALSE);
  g_return_val_if_fail(name != NULL, FALSE);
  g_return_val_if_fail(size > 0, FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);
  klass = NTK_RENDERER_GET_CLASS(self);

  if (klass->get_font == NULL) {
    ntk_error_set_bad_renderer(error, "the render type does not implement get_font", self);
    return FALSE;
  }

  return klass->get_font(self, name, size, error);
}
