#pragma once

#include <ntk/font/pango/build.h>

#ifndef NTK_PANGO_HAS_FT2
#error "NtkPango was not compiled with FT2 support"
#endif

#include "ft2/user.h"
#include "ft2/object.h"
