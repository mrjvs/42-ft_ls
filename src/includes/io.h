#ifndef FTLS_IO
# define FTLS_IO

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bool.h"
#include "context.h"
#include "list.h"

typedef struct s_ftls_file_info {
	struct stat	stat;
	char		*path;
	char		*name;
	t_bool		is_dir;
	t_bool		is_relative;
	t_bool		has_sorting_priority;
	t_bool		is_dotfile;
} ftls_file_info;

struct s_ftls_dir_entry {
	l_list			head;
	ftls_file_info	file;
};

typedef struct s_ftls_dir {
	l_list	files;
	char	*name;
} ftls_dir;

t_bool	gather_directory(ftls_context *ctx, char *path, ftls_dir *out);
t_bool	gather_composed_directory(ftls_context *ctx, int argc, char **argv, ftls_dir *out);

t_bool	retrieve_file_info(ftls_context *ctx, char *path, char *name, ftls_file_info *out);
t_bool	gather_and_print(ftls_context *ctx, int argc, char **argv);

void	free_file_info(ftls_file_info *file);
void	free_directory(ftls_dir *dir);

void	sort_directory(ftls_context *ctx, ftls_dir *dir);

#endif
