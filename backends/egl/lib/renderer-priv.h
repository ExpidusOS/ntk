#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <ntk/backend/egl/renderer.h>
#include <ntk/hw.h>

typedef struct _NtkEGLRendererPrivate {
  NtkHWDisplay* device;
  EGLDisplay* display;
  struct nk_font_atlas atlas;

  struct {
    gboolean KHR_platform_gbm;
    gboolean EXT_platform_device;
    gboolean EXT_device_query;
  } exts;

  struct {
    PFNEGLDEBUGMESSAGECONTROLKHRPROC eglDebugMessageControlKHR;
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
    PFNEGLQUERYDISPLAYATTRIBEXTPROC eglQueryDisplayAttribEXT;
    PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT;
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT;
  } procs;
} NtkEGLRendererPrivate;
