#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <GLES2/gl2.h>
#include <ntk/hw.h>

#ifdef NTK_HW_HAS_LIBDRM
typedef struct {
	uint32_t drm_format;
	GLint gl_internalformat;
	GLint gl_format, gl_type;
	bool has_alpha;
} NtkGLES2PixelFormat;

const NtkGLES2PixelFormat* ntk_gles2_pixel_format_for_drm(uint32_t fmt);
#endif
