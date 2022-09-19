#pragma once

#include <ntk/font/pango/build.h>

#ifndef NTK_PANGO_HAS_FT2
#error "NtkPango was not compiled with FT2 support"
#endif

#include <glib-object.h>
#include <pango/pango.h>
#include <ntk/font/user.h>

#define NTK_PANGO_TYPE_FT2_FONT ntk_pango_ft2_font_get_type()

/**
 * NtkPangoFT2Font:
 *
 * Since: 0.1.0
 */
typedef struct _NtkPangoFT2Font {
  NtkUserFont parent_instance;

  /*< private >*/
  struct _NtkPangoFT2FontPrivate* priv;
} NtkPangoFT2Font;

GType ntk_pango_ft2_font_get_type();

NtkPangoFT2Font* ntk_pango_ft2_font_new(PangoFontDescription* desc, NtkFontUploadGPUCallback gpu_upload, gpointer gpu_upload_target);

NtkPangoFT2Font* ntk_pango_ft2_font_copy(NtkPangoFT2Font* self);
void ntk_pango_ft2_font_free(NtkPangoFT2Font* self);
