#pragma once

#include <ntk/font.h>
#include <ntk/renderer.h>
#include <nuklear.h>

typedef struct _NtkContextPrivate {
  gboolean inited;
  gboolean is_drawing;
  struct nk_context nk;
  NtkRenderer* renderer;
  NtkFont* font;
} NtkContextPrivate;
