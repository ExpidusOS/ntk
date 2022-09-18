#pragma once

#include <drm/drm.h>
#include <xf86drm.h>

typedef struct _NtkHWDrmDisplayPrivate {
  drmDevicePtr device;
} NtkHWDrmDisplayPrivate;
