#pragma once

#include <pango/pangoft2.h>
#include <ntk/font.h>
#include <nuklear.h>

typedef struct _NtkPangoFT2FontPrivate {
  PangoFont* font;
  PangoContext* ctx;
  struct nk_font_atlas atlas;
  struct nk_font* nk_font;
  struct nk_font_config cfg;
  struct nk_draw_null_texture null;
  struct nk_image img;
  NtkFontUploadGPUCallback gpu_upload;
  gpointer gpu_upload_target;
} NtkPangoFT2FontPrivate;
