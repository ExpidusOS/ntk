#ifndef __NTK_RENDERER_H_
#define __NTK_RENDERER_H_ 1

#include <glib-object.h>
#include <nuklear.h>
#include <pango/pango.h>

G_BEGIN_DECLS

#define NTK_TYPE_RENDERER ntk_renderer_get_type()
G_DECLARE_INTERFACE(NtkRenderer, ntk_renderer, NTK, RENDERER, GObject);

typedef enum { /*< flags,prefix=NTK_RENDERER_TYPE,since=0.1.0 >*/
	NTK_RENDERER_TYPE_COMMAND = 0,
	NTK_RENDERER_TYPE_VERTEX = 1
} NtkRendererType;

struct _NtkRendererInterface {
	GTypeInterface parent_iface;

	NtkRendererType (*get_render_type)(NtkRenderer* self);
	gboolean (*render_command)(NtkRenderer* self, struct nk_command* cmd, GError** error);
	gboolean (*render_vertex)(NtkRenderer* self, struct nk_draw_command* cmd, GError** error);
	struct nk_user_font* (*get_font)(NtkRenderer* self, PangoFontDescription* desc, GError** error);
};

NtkRendererType ntk_renderer_get_render_type(NtkRenderer* self);
gboolean ntk_renderer_draw(NtkRenderer* self, struct nk_command* cmd, struct nk_draw_command* draw_cmd, GError** error);
struct nk_user_font* ntk_renderer_get_font(NtkRenderer* self, PangoFontDescription* desc, GError** error);

G_END_DECLS

#endif
