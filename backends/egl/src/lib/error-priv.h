#ifndef __ERROR_PRIV_H_
#define __ERROR_PRIV_H_ 1

#include <ntk/backend/egl/error.h>
#include <EGL/egl.h>

void ntk_egl_error_set_bad_proc(GError** error, const char* reason, const char* name);
void ntk_egl_error_set_missing_ext(GError** error, const char* reason, const char* name);
void ntk_egl_error_set_binding(GError** error, const char* reason);
void ntk_egl_error_set_egl(GError** error, const char* reason, EGLint e);

#endif
