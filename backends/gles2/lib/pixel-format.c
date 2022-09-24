#include "pixel-format-priv.h"

#ifdef NTK_HW_HAS_LIBDRM
#include <GLES2/gl2ext.h>
#include <drm_fourcc.h>

static const NtkGLES2PixelFormat formats[] = {
  {
    .drm_format = DRM_FORMAT_ARGB8888,
    .gl_format = GL_BGRA_EXT,
    .gl_type = GL_UNSIGNED_BYTE,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_XRGB8888,
    .gl_format = GL_BGRA_EXT,
    .gl_type = GL_UNSIGNED_BYTE,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_XBGR8888,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_BYTE,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_ABGR8888,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_BYTE,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_BGR888,
    .gl_format = GL_RGB,
    .gl_type = GL_UNSIGNED_BYTE,
    .has_alpha = false,
  },
#if LITTLE_ENDIAN
  {
    .drm_format = DRM_FORMAT_RGBX4444,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT_4_4_4_4,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_RGBA4444,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT_4_4_4_4,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_RGBX5551,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT_5_5_5_1,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_RGBA5551,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT_5_5_5_1,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_RGB565,
    .gl_format = GL_RGB,
    .gl_type = GL_UNSIGNED_SHORT_5_6_5,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_XBGR2101010,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_INT_2_10_10_10_REV_EXT,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_ABGR2101010,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_INT_2_10_10_10_REV_EXT,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_XBGR16161616F,
    .gl_format = GL_RGBA,
    .gl_type = GL_HALF_FLOAT_OES,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_ABGR16161616F,
    .gl_format = GL_RGBA,
    .gl_type = GL_HALF_FLOAT_OES,
    .has_alpha = true,
  },
  {
    .drm_format = DRM_FORMAT_XBGR16161616,
    .gl_internalformat = GL_RGBA16_EXT,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT,
    .has_alpha = false,
  },
  {
    .drm_format = DRM_FORMAT_ABGR16161616,
    .gl_internalformat = GL_RGBA16_EXT,
    .gl_format = GL_RGBA,
    .gl_type = GL_UNSIGNED_SHORT,
    .has_alpha = true,
  },
#endif
};

const NtkGLES2PixelFormat* ntk_gles2_pixel_format_for_drm(uint32_t fmt) {
  for (size_t i = 0; i < sizeof(formats) / sizeof(*formats); ++i) {
    if (formats[i].drm_format == fmt) return &formats[i];
  }
  return NULL;
}
#endif
