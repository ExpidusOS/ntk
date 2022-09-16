#pragma once

#include <glib-object.h>
#include <nuklear.h>

G_BEGIN_DECLS

#define NTK_TYPE_USER_FONT ntk_user_font_get_type()

/**
 * NtkUserFont:
 *
 * Since: 0.1.0
 */
typedef struct _NtkUserFont {
  nk_handle userdata;
  float height;
  nk_text_width_f width;
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
  nk_query_font_glyph_f query;
  nk_handle texture;
#endif

  struct _NtkUserFont* (*impl_copy)(struct _NtkUserFont* self);
  void (*impl_free)(struct _NtkUserFont* self);
} NtkUserFont;

GType ntk_user_font_get_type();

NtkUserFont* ntk_user_font_copy(NtkUserFont* self);
void ntk_user_font_free(NtkUserFont* self);

G_END_DECLS
