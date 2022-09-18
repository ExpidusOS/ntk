#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <ntk/backend/egl/renderer.h>
#include <ntk/hw.h>

typedef struct _NtkEGLRendererPrivate {
  NtkHWDisplay* device;
  EGLDeviceEXT device_attrib;
  EGLDisplay* display;
  EGLContext context;
  EGLSurface surface;
  EGLConfig config;
  struct nk_font_atlas atlas;
  int width;
  int height;

  struct {
    gboolean KHR_image_base;
    gboolean KHR_platform_gbm;
    gboolean EXT_platform_device;
    gboolean EXT_device_query;
    gboolean IMG_context_priority;
#ifdef NTK_HW_HAS_LIBDRM
    gboolean EXT_device_drm;
    gboolean EXT_device_drm_render_node;
#endif
  } exts;

  struct {
    PFNEGLDEBUGMESSAGECONTROLKHRPROC eglDebugMessageControlKHR;
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
    PFNEGLQUERYDISPLAYATTRIBEXTPROC eglQueryDisplayAttribEXT;
    PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT;
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT;
    PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;
    PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
  } procs;
} NtkEGLRendererPrivate;
