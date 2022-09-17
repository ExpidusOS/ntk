#pragma once

#include "user.h"
#include <glib-object.h>
#include <nuklear.h>

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
  gchar* (*get_name)(NtkFont* self);
};

NtkUserFont* ntk_font_get_handle(NtkFont* self);
gchar* ntk_font_get_name(NtkFont* self);

G_END_DECLS
