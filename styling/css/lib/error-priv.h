#pragma once

#include <cssparser.h>
#include <ntk/styling/css/error.h>

void ntk_css_error_set(GError** error, CssError* cerror);
