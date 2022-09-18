#pragma once

#include <ntk/hw/build.h>

#ifndef NTK_HW_HAS_LIBDRM
#error "Libdrm support was not compiled into NTK"
#endif

#include "drm/render.h"
