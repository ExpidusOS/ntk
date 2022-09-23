#pragma once

#include <ntk/styling/css/error.h>
#include <cssparser.h>

void ntk_css_error_set(GError** error, CssError* cerror);
