#pragma once

#include <drm/drm.h>
#include <glib.h>
#include <xf86drm.h>

typedef struct _NtkHWDrmRenderPrivate {
  drmDevicePtr device;
  int fd;
  gchar* path;
} NtkHWDrmRenderPrivate;
