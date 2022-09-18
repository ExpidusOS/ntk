#pragma once

#include <drm/drm.h>
#include <xf86drm.h>
#include <glib.h>

typedef struct _NtkHWDrmRenderPrivate {
  drmDevicePtr device;
  int fd;
  gchar* path;
} NtkHWDrmRenderPrivate;
