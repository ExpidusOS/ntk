#pragma once

#include <GLES2/gl2.h>
#include <glib.h>

void ntk_gles2_error_set_missing_ext(GError** error, const char* reason, const char* name);
void ntk_gles2_error_set_gl(GError** error, const char* reason, GLint e);
