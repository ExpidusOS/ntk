#pragma once

#include "hw/device.h"
#include "hw/display.h"
#include "hw/input.h"
#include <ntk/hw/build.h>

#ifdef NTK_HW_HAS_LIBDRM
#include "hw/device/drm.h"
#endif
