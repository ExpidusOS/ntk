#ifndef __RENDERER_PRIV_H_
#define __RENDERER_PRIV_H_ 1

#include <ntk/backend/egl/renderer.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

typedef struct _NtkEGLRendererPrivate {
  EGLDisplay* egl_display;
  struct nk_font_atlas atlas;

  struct {
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
  } procs;
} NtkEGLRendererPrivate;

#endif
