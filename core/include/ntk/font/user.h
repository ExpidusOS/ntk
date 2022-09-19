#pragma once

#include <glib-object.h>
#include <nuklear.h>

G_BEGIN_DECLS

#define NTK_TYPE_USER_FONT ntk_user_font_get_type()

typedef struct nk_image (*NtkFontUploadGPUCallback)(struct nk_image* src, gpointer userdata);

/**
 * NtkUserFont:
 *
 * Since: 0.1.0
 */
typedef struct _NtkUserFont {
  struct nk_user_font parent_instance;

  struct _NtkUserFont* (*impl_copy)(struct _NtkUserFont* self);
  void (*impl_free)(struct _NtkUserFont* self);
} NtkUserFont;

GType ntk_user_font_get_type();

NtkUserFont* ntk_user_font_copy(NtkUserFont* self);
void ntk_user_font_free(NtkUserFont* self);

G_END_DECLS
