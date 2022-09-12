#define G_LOG_DOMAIN "NtkRendererGtk4"
#include <ntk/renderer/gtk4/renderer.h>
#include <ntk/font.h>
#include <ntk/utils.h>
#include <math.h>
#include "renderer-priv.h"

#define NTK_RENDERER_GTK4_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_renderer_gtk4_renderer_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkRendererGtk4Renderer, ntk_renderer_gtk4_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_renderer_gtk4_renderer_get_render_type(NtkRenderer* renderer) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);
  return ntk_renderer_get_render_type(priv->renderer);
}

static gboolean ntk_renderer_gtk4_renderer_render_command(NtkRenderer* renderer, const struct nk_command* cmd, GError** error) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  NtkRendererClass* renderer_class = NTK_RENDERER_GET_CLASS(priv->renderer);
  g_return_val_if_fail(renderer_class != NULL, FALSE);
  g_return_val_if_fail(renderer_class->render_command != NULL, FALSE);
  return renderer_class->render_command(priv->renderer, cmd, error);
}

static struct nk_user_font* ntk_renderer_gtk4_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);
  return ntk_renderer_get_font(priv->renderer, desc, error);
}

static void ntk_renderer_gtk4_renderer_get_size(NtkRenderer* renderer, int* width, int* height) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  ntk_renderer_get_size(priv->renderer, width, height);
}

static void ntk_renderer_gtk4_renderer_set_size(NtkRenderer* renderer, int width, int height) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(renderer);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  ntk_renderer_set_size(priv->renderer, width, height);
}

static void ntk_renderer_gtk4_renderer_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_renderer_gtk4_renderer_parent_class)->constructed(obj);

  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  priv->renderer = ntk_renderer_cairo_renderer_new(1, 1);
}

static void ntk_renderer_gtk4_renderer_finalize(GObject* obj) {
  NtkRendererGtk4Renderer* self = NTK_RENDERER_GTK4_RENDERER(obj);
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  g_clear_object(&priv->renderer);

  G_OBJECT_CLASS(ntk_renderer_gtk4_renderer_parent_class)->finalize(obj);
}

static void ntk_renderer_gtk4_renderer_class_init(NtkRendererGtk4RendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->constructed = ntk_renderer_gtk4_renderer_constructed;
  object_class->finalize = ntk_renderer_gtk4_renderer_finalize;

  renderer_class->get_render_type = ntk_renderer_gtk4_renderer_get_render_type;
  renderer_class->render_command = ntk_renderer_gtk4_renderer_render_command;
  renderer_class->get_font = ntk_renderer_gtk4_renderer_get_font;
  renderer_class->get_size = ntk_renderer_gtk4_renderer_get_size;
  renderer_class->set_size = ntk_renderer_gtk4_renderer_set_size;
}

static void ntk_renderer_gtk4_renderer_init(NtkRendererGtk4Renderer* self) {
  self->priv = ntk_renderer_gtk4_renderer_get_instance_private(self);
}

NtkRenderer* ntk_renderer_gtk4_renderer_new() {
  return NTK_RENDERER(g_object_new(NTK_RENDERER_GTK4_TYPE_RENDERER, NULL));
}

void ntk_renderer_gtk4_renderer_snapshot_to(NtkRendererGtk4Renderer* self, GtkSnapshot* snapshot) {
  g_return_if_fail(NTK_RENDERER_GTK4_IS_RENDERER(self));
  NtkRendererGtk4RendererPrivate* priv = NTK_RENDERER_GTK4_RENDERER_PRIVATE(self);

  NtkRendererCairoRenderer* renderer = NTK_RENDERER_CAIRO_RENDERER(priv->renderer);
  g_assert(renderer != NULL);

  cairo_surface_t* surf = ntk_renderer_cairo_renderer_get_surface(renderer);

  int width;
  int height;
  ntk_renderer_get_size(priv->renderer, &width, &height);

  cairo_t* cr = gtk_snapshot_append_cairo(snapshot, &GRAPHENE_RECT_INIT(0, 0, width, height));
  cairo_set_source_surface(cr, surf, 0, 0);
  cairo_paint(cr);
  cairo_destroy(cr);
}
