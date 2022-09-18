#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <gio/gio.h>
#include <ntk/backend/gles2/renderer.h>
#include "renderer-priv.h"

#define NTK_GLES2_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_gles2_renderer_get_instance_private(self) : (self)->priv)

enum {
  SIG_RENDER,
  N_SIGNALS
};

static guint obj_sigs[N_SIGNALS] = {0};

static void ntk_gles2_renderer_interface_init(GInitableIface* iface);

G_DEFINE_TYPE_WITH_CODE(
  NtkGLES2Renderer, ntk_gles2_renderer, NTK_TYPE_RENDERER,
  G_ADD_PRIVATE(NtkGLES2Renderer) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, ntk_gles2_renderer_interface_init)
);

static gboolean ntk_gles2_renderer_has_ext(NtkGLES2Renderer* self, const char* ext) {
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);
  const gchar* exts = priv->exts;

  size_t extlen = strlen(ext);
  const char* end = exts + strlen(exts);
  while (exts < end) {
    if (*exts == ' ') {
      exts++;
      continue;
    }
    size_t n = strcspn(exts, " ");
    if (n == extlen && strncmp(ext, exts, n) == 0) return TRUE;
    exts += n;
  }
  return FALSE;
}

static void ntk_gles2_renderer_finalize(GObject* obj) {
  G_OBJECT_CLASS(ntk_gles2_renderer_parent_class)->finalize(obj);
}

static gboolean ntk_gles2_renderer_initable_init(GInitable* initable, GCancellable* cancellable, GError** error) {
  NtkGLES2Renderer* self = NTK_GLES2_RENDERER(initable);
  NtkGLES2RendererPrivate* priv = NTK_GLES2_RENDERER_PRIVATE(self);

  priv->exts = (const char*)glGetString(GL_EXTENSIONS);

  g_debug("Queried GL extensions: %s", priv->exts);
  return TRUE;
}

static void ntk_gles2_renderer_interface_init(GInitableIface* iface) {
  iface->init = ntk_gles2_renderer_initable_init;
}

static void ntk_gles2_renderer_class_init(NtkGLES2RendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->finalize = ntk_gles2_renderer_finalize;
}

static void ntk_gles2_renderer_init(NtkGLES2Renderer* self) {
  self->priv = ntk_gles2_renderer_get_instance_private(self);
}

NtkRenderer* ntk_gles2_renderer_new(GError** error) {
  return NTK_RENDERER(g_initable_new(NTK_GLES2_TYPE_RENDERER, NULL, error, NULL));
}
