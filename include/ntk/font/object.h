#ifndef __NTK_FONT_OBJECT_H_
#define __NTK_FONT_OBJECT_H_ 1

#include <glib-object.h>
#include <nuklear.h>
#include "user.h"

G_BEGIN_DECLS

#define NTK_TYPE_FONT ntk_font_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkFont, ntk_font, NTK, FONT, GObject);

/**
 * NtkFont:
 *
 * Since: 0.1.0
 */
struct _NtkFontClass {
  GObjectClass parent_class;

  NtkUserFont* (*get_handle)(NtkFont* self);
};

NtkUserFont* ntk_font_get_handle(NtkFont* self);

G_END_DECLS

#endif
