#pragma once

#include "hw/error.h"
#include "hw/device.h"
#include "hw/display.h"
#include "hw/input.h"
#include <ntk/hw/build.h>
#include <ntk/ntk-hw-enums.h>

#ifdef NTK_HW_HAS_LIBDRM
#include "hw/drm.h"
#endif
