#ifndef __NTK_FONT_USER_H_
#define __NTK_FONT_USER_H_ 1

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
  struct nk_user_font;

  struct _NtkUserFont* (*copy)(struct _NtkUserFont* self);
  void (*free)(struct _NtkUserFont* self);
} NtkUserFont;

GType ntk_user_font_get_type();

NtkUserFont* ntk_user_font_copy(NtkUserFont* self);
void ntk_user_font_free(NtkUserFont* self);

G_END_DECLS

#endif
