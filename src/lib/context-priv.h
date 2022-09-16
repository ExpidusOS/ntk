#ifndef __CONTEXT_PRIVATE_H_
#define __CONTEXT_PRIVATE_H_ 1

#include <ntk/renderer.h>
#include <nuklear.h>

typedef struct _NtkContextPrivate {
  gboolean inited;
  gboolean is_drawing;
  struct nk_context nk;
  NtkRenderer* renderer;
  PangoFontDescription* font_desc;
} NtkContextPrivate;

#endif
