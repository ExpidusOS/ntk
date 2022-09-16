#pragma once

#include <pango/pango.h>

typedef struct _NtkPangoFontPrivate {
  PangoFontDescription* desc;
  PangoContext* ctx;
} NtkPangoFontPrivate;
