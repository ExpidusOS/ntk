#ifndef __ERROR_PRIV_H_
#define __ERROR_PRIV_H_ 1

#include <ntk/context.h>
#include <ntk/error.h>
#include <ntk/renderer.h>

void ntk_error_set_bad_renderer(GError** error, const char* reason, NtkRenderer* renderer);
void ntk_error_set_nuklear_fail(GError** error, const char* reason, NtkContext* context);

#endif
