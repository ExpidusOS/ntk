#define G_LOG_DOMAIN "NtkGtk4Renderer"
#include "renderer-priv.h"
#include <math.h>
#include <ntk/backend/gtk4/renderer.h>
#include <ntk/font.h>
#include <ntk/utils.h>

#define NTK_GTK4_RENDERER_PRIVATE(self) ((self)->priv == NULL ? ntk_gtk4_renderer_get_instance_private(self) : (self)->priv)

G_DEFINE_TYPE_WITH_PRIVATE(NtkGtk4Renderer, ntk_gtk4_renderer, NTK_TYPE_RENDERER);

static NtkRendererType ntk_gtk4_renderer_get_render_type(NtkRenderer* renderer) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(renderer);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);
  return ntk_renderer_get_render_type(priv->renderer);
}

static gboolean ntk_gtk4_renderer_render_command(NtkRenderer* renderer, const struct nk_command* cmd, GError** error) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(renderer);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  NtkRendererClass* renderer_class = NTK_RENDERER_GET_CLASS(priv->renderer);
  g_return_val_if_fail(renderer_class != NULL, FALSE);
  g_return_val_if_fail(renderer_class->render_command != NULL, FALSE);
  return renderer_class->render_command(priv->renderer, cmd, error);
}

static struct nk_user_font* ntk_gtk4_renderer_get_font(NtkRenderer* renderer, PangoFontDescription* desc, GError** error) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(renderer);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);
  return ntk_renderer_get_font(priv->renderer, desc, error);
}

static void ntk_gtk4_renderer_get_size(NtkRenderer* renderer, int* width, int* height) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(renderer);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  ntk_renderer_get_size(priv->renderer, width, height);
}

static void ntk_gtk4_renderer_set_size(NtkRenderer* renderer, int width, int height) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(renderer);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  ntk_renderer_set_size(priv->renderer, width, height);
}

static void ntk_gtk4_renderer_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_gtk4_renderer_parent_class)->constructed(obj);

  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(obj);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  priv->renderer = ntk_cairo_renderer_new(1, 1);
}

static void ntk_gtk4_renderer_finalize(GObject* obj) {
  NtkGtk4Renderer* self = NTK_GTK4_RENDERER(obj);
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  g_clear_object(&priv->renderer);

  G_OBJECT_CLASS(ntk_gtk4_renderer_parent_class)->finalize(obj);
}

static void ntk_gtk4_renderer_class_init(NtkGtk4RendererClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  NtkRendererClass* renderer_class = NTK_RENDERER_CLASS(klass);

  object_class->constructed = ntk_gtk4_renderer_constructed;
  object_class->finalize = ntk_gtk4_renderer_finalize;

  renderer_class->get_render_type = ntk_gtk4_renderer_get_render_type;
  renderer_class->render_command = ntk_gtk4_renderer_render_command;
  renderer_class->get_font = ntk_gtk4_renderer_get_font;
  renderer_class->get_size = ntk_gtk4_renderer_get_size;
  renderer_class->set_size = ntk_gtk4_renderer_set_size;
}

static void ntk_gtk4_renderer_init(NtkGtk4Renderer* self) {
  self->priv = ntk_gtk4_renderer_get_instance_private(self);
}

NtkRenderer* ntk_gtk4_renderer_new() {
  return NTK_RENDERER(g_object_new(NTK_GTK4_TYPE_RENDERER, NULL));
}

void ntk_gtk4_renderer_snapshot_to(NtkGtk4Renderer* self, GtkSnapshot* snapshot) {
  g_return_if_fail(NTK_GTK4_IS_RENDERER(self));
  NtkGtk4RendererPrivate* priv = NTK_GTK4_RENDERER_PRIVATE(self);

  NtkCairoRenderer* renderer = NTK_CAIRO_RENDERER(priv->renderer);
  g_assert(renderer != NULL);

  cairo_surface_t* surf = ntk_cairo_renderer_get_surface(renderer);

  int width;
  int height;
  ntk_renderer_get_size(priv->renderer, &width, &height);

  cairo_t* cr = gtk_snapshot_append_cairo(snapshot, &GRAPHENE_RECT_INIT(0, 0, width, height));
  cairo_set_source_surface(cr, surf, 0, 0);
  cairo_paint(cr);
  cairo_destroy(cr);
}
