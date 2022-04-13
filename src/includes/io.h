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
int		gather_and_print(ftls_context *ctx, int argc, char **argv);

#endif
