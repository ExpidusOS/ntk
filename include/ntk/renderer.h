#ifndef __NTK_RENDERER_H_
#define __NTK_RENDERER_H_ 1

#include <glib-object.h>
#include <nuklear.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define NTK_TYPE_RENDERER ntk_renderer_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkRenderer, ntk_renderer, NTK, RENDERER, GObject);

/**
 * NtkRendererType:
 *
 * Since: 0.1.0
 */
typedef enum { /*< flags,prefix=NTK_RENDERER_TYPE,since=0.1.0 >*/
  NTK_RENDERER_TYPE_COMMAND = 0,
  NTK_RENDERER_TYPE_VERTEX = 1
} NtkRendererType;

typedef struct nk_command NtkRendererDrawCommand;

typedef struct {
  const struct nk_draw_command* cmd;
  struct nk_buffer cmds;
  struct nk_buffer verts;
  struct nk_buffer idx;
} NtkRendererVertexCommand;

typedef struct {
  int is_vertex:1;
  union {
    const NtkRendererDrawCommand* draw;
    NtkRendererVertexCommand vertex;
  };
} NtkRendererCommand;

/**
 * NtkRenderer:
 *
 * Since: 0.1.0
 */
struct _NtkRendererClass {
  GObjectClass parent_class;

  NtkRendererType (*get_render_type)(NtkRenderer* self);
  gboolean (*render_command)(NtkRenderer* self, const NtkRendererDrawCommand* cmd, GError** error);
  gboolean (*render_vertex)(NtkRenderer* self, NtkRendererVertexCommand* cmd, GError** error);
  struct nk_user_font* (*get_font)(NtkRenderer* self, PangoFontDescription* desc, GError** error);
  void (*set_size)(NtkRenderer* self, int width, int height);
  void (*get_size)(NtkRenderer* self, int* width, int* height);
};

void ntk_renderer_request_draw(NtkRenderer* self);
void ntk_renderer_set_size(NtkRenderer* self, int width, int height);
void ntk_renderer_get_size(NtkRenderer* self, int* width, int* height);
NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self);
gboolean ntk_renderer_draw(NtkRenderer* self, NtkRendererCommand* cmd, GError** error);
struct nk_user_font* ntk_renderer_get_font(NtkRenderer* self, PangoFontDescription* desc, GError** error);

G_END_DECLS

#endif
