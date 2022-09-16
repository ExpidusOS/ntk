#ifndef __NTK_UTILS_H_
#define __NTK_UTILS_H_ 1

#include <glib.h>
#include <nuklear.h>

void ntk_command_log(
  const gchar* domain, GLogLevelFlags log_level, const gchar* file, const gchar* line, const gchar* func,
  const struct nk_command* cmd
);

#define ntk_command_debug(cmd)                                                                                              \
  ntk_command_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, __FILE__, G_STRINGIFY(__LINE__), G_STRFUNC, cmd)

#endif
