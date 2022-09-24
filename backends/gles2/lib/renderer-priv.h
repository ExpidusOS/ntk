#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glib.h>
#include <ntk/backend/gles2/renderer.h>

typedef struct _NtkGLES2RendererPrivate {
  const char* exts_str;
  NtkGLES2RendererLoadProcHandler proc_handler;

  struct {
    gboolean EXT_read_format_bgra;
    gboolean EXT_texture_type_2_10_10_10_REV;
    gboolean OES_texture_half_float_linear;
    gboolean EXT_texture_norm16;
    gboolean KHR_debug;
    gboolean OES_egl_image;
  } exts;

  struct {
    PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC glEGLImageTargetRenderbufferStorageOES;
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
    PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallbackKHR;
    PFNGLDEBUGMESSAGECONTROLKHRPROC glDebugMessageControlKHR;
  } procs;
} NtkGLES2RendererPrivate;
