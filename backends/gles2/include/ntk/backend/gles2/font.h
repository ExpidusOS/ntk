#pragma once

#include <glib-object.h>
#include <ntk/font/pango.h>

#define NTK_GLES2_TYPE_FONT ntk_gles2_font_get_type()
G_DECLARE_FINAL_TYPE(NtkGLES2Font, ntk_gles2_font, NTK_GLES2, FONT, NtkPangoFont);

/**
 * NtkGLES2Font:
 *
 * Since: 0.1.0
 */
struct _NtkGLES2Font {
  NtkPangoFont parent_instance;
};

NtkFont* ntk_gles2_font_layout_new(PangoFontDescription* desc);
