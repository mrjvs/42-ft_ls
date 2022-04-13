#include "context.h"
#include "io.h"
#include <string.h>

/**
 * Gather file information from cli inputs
 * then print them the same way LS prints and according to context
 * 
 * returns true is success
 */
int	gather_and_print(ftls_context *ctx, int argc, char **argv)
{
	// TODO temp strdup + no error handling + memleak
	char *first_arg = NULL;
	t_bool has_one_arg = argc <= 1;
	if (argc < 1)
		first_arg = strdup(".");
	else if (argc == 1)
		first_arg = strdup(argv[0]);
	
	// if only one argument and its a directory
	if (has_one_arg)
	{
		ftls_file_info file;
		if (!retrieve_file_info(ctx, first_arg, &file))
			return false; // TODO add error handling
		if (file.is_dir)
		{
			ftls_dir dir;
			gather_directory(ctx, first_arg, &dir);
			// TODO handle errors
			// gather_directory(first_arg);

			// print_directory(prefix = false, full = true, hide_directories = false, recurse = 0);
			return true;
		}
	}

	// treat as list of files and directories
	// gather_composed_directory(argv);
	// print_directory(prefix = false, full = false, hide_directories = true, recurse = 1)
	return true;
}
