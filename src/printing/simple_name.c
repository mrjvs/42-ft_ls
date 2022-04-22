#include "context.h"
#include "ftls_string.h"
#include "io.h"
#include <unistd.h>

/**
 * print file name with ls based colors pulled from context
*/
void	print_simple_name(ftls_context *ctx, ftls_file_info *file) {
	ftls_write(STDERR_FILENO, "\e[0m");
	ftls_write(STDERR_FILENO, ctx->colors[file->color][0]);
	ftls_write(STDERR_FILENO, ctx->colors[file->color][1]);
	ftls_write(STDOUT_FILENO, file->name);
	ftls_write(STDERR_FILENO, "\e[0m");
}
