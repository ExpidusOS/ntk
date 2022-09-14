#ifndef __FONT_PANGO_OBJECT_PRIV_H_
#define __FONT_PANGO_OBJECT_PRIV_H_ 1

#include <pango/pango.h>

typedef struct _NtkPangoFontPrivate {
  PangoFontDescription* desc;
} NtkPangoFontPrivate;

#endif
