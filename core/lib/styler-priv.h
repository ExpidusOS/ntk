#pragma once

#include <ntk/styler.h>
#include <nuklear.h>

typedef struct _NtkStylerPrivate {
  struct nk_style saved_style;
  GHashTable* styles;
} NtkStylerPrivate;
