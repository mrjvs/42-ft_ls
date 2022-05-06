#ifndef FTLS_PRINTING
# define FTLS_PRINTING

#include "bool.h"
#include "context.h"
#include "io.h"
#include <stddef.h>

#define FTLS_TIME_TO_SHOW_YEAR 15780000

typedef struct s_ftls_print_options {
	t_bool	show_prefix;	// show prefix of directory
	t_bool	display_full;	// show directories as well as files
	t_bool	force_compose;	// force display of compose: hides directories and shows dotfiles in directory recurse 
	int		recurse;		// how many times to recurse, -1 for infinite, gets overwritten by context
	t_bool	show_total;		// show total block count
}	ftls_print_options;

struct s_ftls_col {
	char			*str;
	t_bool			exists;
	ftls_file_info	file;
	t_bool			right_align;
	t_bool			name;
};

int		should_print_file(ftls_context *ctx, ftls_file_info *file);

void	print_directory(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops);
void	print_grid(ftls_context *ctx, ftls_dir *dir, int columns, int *sizes);
void	print_long_format(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops);

void	print_access_error(ftls_context *ctx, char *path);
void	print_directory_error(ftls_context *ctx, const char *path);
void	print_path_error(ftls_context *ctx, const char *path, const char *msg);
void	print_error(ftls_context *ctx, const char *str);
void	print_errno(ftls_context *ctx);

int		max_columns_for_files(ftls_context *ctx, ftls_dir *dir, int **sizes);
void	print_simple_name(ftls_context *ctx, ftls_file_info *file);
char	*get_perms(ftls_file_info *file);

#endif
