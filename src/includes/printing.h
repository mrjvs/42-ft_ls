#ifndef FTLS_PRINTING
# define FTLS_PRINTING

#include "bool.h"
#include "context.h"
#include "io.h"

#define should_print_file(ctx, file) (!file.is_dotfile || ctx->ops.list_all)

typedef struct s_ftls_print_options {
	t_bool	show_prefix;	// show prefix of directory
	t_bool	display_full;	// ??
	t_bool	force_compose;	// force display of compose: hides directories and shows dotfiles in directory recurse 
	int		recurse;		// how many times to recurse, -1 for infinite, gets overwritten by context
}	ftls_print_options;

void	print_directory(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops);

void	print_access_error(ftls_context *ctx, char *path);
void	print_directory_error(ftls_context *ctx, const char *path);
void	print_path_error(ftls_context *ctx, const char *path, const char *msg);
void	print_error(ftls_context *ctx, const char *str);
void	print_errno(ftls_context *ctx);

#endif
