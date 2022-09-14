#ifndef __ERROR_PRIV_H_
#define __ERROR_PRIV_H_ 1

#include <ntk/backend/egl/error.h>

void ntk_egl_error_set_bad_proc(GError** error, const char* reason, const char* name);

#endif
