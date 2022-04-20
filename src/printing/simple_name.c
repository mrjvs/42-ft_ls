#include "context.h"
#include "ftls_string.h"
#include "io.h"

int	print_simple_name(ftls_context *ctx, ftls_file_info *file) {
	// TODO figure out colors
	(void)ctx;
	ftls_write(STDOUT_FILENO, file->name);
	return 1;
}
