#ifndef FTLS_PRINTING
# define FTLS_PRINTING

#include "bool.h"
#include "context.h"
#include "io.h"

typedef struct s_ftls_print_options {
	t_bool	show_prefix;	// show prefix of directory
	t_bool	display_full;	// ??
	t_bool	hide_dirs;		// hide directories on first iteration, gets ignored after one recurse
	int		recurse;		// how many times to recurse, -1 for infinite, gets overwritten by context
}	ftls_print_options;

void	print_directory(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops);

#endif
