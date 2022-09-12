#define G_LOG_DOMAIN "NtkUtils"
#include <ntk/utils.h>

void ntk_command_log(const gchar* domain, GLogLevelFlags log_level, const gchar* file, const gchar* line, const gchar* func, const struct nk_command* cmd) {
#define LOG(t, fmt, ...) g_log_structured_standard(domain, log_level, file, line, func, "NK_COMMAND_" #t "(%d): " fmt, cmd->type, __VA_ARGS__)
	switch (cmd->type) {
		case NK_COMMAND_NOP:
			g_log_structured(domain, log_level, file, line, func, "NK_COMMAND_NOP(%d)", cmd->type);
			break;
		case NK_COMMAND_SCISSOR:
			{
				struct nk_command_scissor* c = (struct nk_command_scissor*)cmd;
			}
			break;
		case NK_COMMAND_LINE:
			{
				struct nk_command_line* c = (struct nk_command_line*)cmd;
			}
			break;
		case NK_COMMAND_RECT_FILLED:
			{
				struct nk_command_rect_filled* c = (struct nk_command_rect_filled*)cmd;
				LOG(RECT_FILLED, "rounding: %d, x: %d, y: %d, w: %d, h: %d, color: (r: %d, g: %d, b: %d, a: %d)", c->rounding, c->x, c->y, c->w, c->h, c->color.r, c->color.g, c->color.b, c->color.a);
			}
			break;
	}
}
