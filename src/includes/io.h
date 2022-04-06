#ifndef FTLS_IO
# define FTLS_IO

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bool.h"
#include "context.h"

typedef struct s_ftls_file_info {
	struct stat	stat;
	char		*path;
	t_bool		is_dir;
} ftls_file_info;

t_bool	retrieve_file_info(ftls_context *ctx, char *path, ftls_file_info *out);
int		gather_and_print(ftls_context *ctx, int argc, char **argv);

#endif
