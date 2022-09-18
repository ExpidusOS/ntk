#pragma once

#include <ntk/hw/device.h>
#include <ntk/hw/error.h>

void ntk_hw_error_set_bad_device(GError** error, const char* reason, NtkHWDevice* device);
