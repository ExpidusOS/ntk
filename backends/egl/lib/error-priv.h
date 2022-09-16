#pragma once

#include <EGL/egl.h>
#include <ntk/backend/egl/error.h>

void ntk_egl_error_set_bad_proc(GError** error, const char* reason, const char* name);
void ntk_egl_error_set_missing_ext(GError** error, const char* reason, const char* name);
void ntk_egl_error_set_binding(GError** error, const char* reason);
void ntk_egl_error_set_egl(GError** error, const char* reason, EGLint e);
