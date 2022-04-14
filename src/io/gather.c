#include "context.h"
#include "io.h"
#include "printing.h"
#include "ftls_string.h"

/**
 * Gather file information from cli inputs
 * then print them the same way LS prints and according to context
 * 
 * returns true if success
 */
t_bool	gather_and_print(ftls_context *ctx, int argc, char **argv)
{
	char *first_arg = NULL;
	t_bool has_one_arg = argc <= 1;
	if (argc < 1)
		first_arg = ftls_strdup(".");
	else if (argc == 1)
		first_arg = ftls_strdup(argv[0]);
	if (has_one_arg && !first_arg) {
		print_errno(ctx);
		ctx->major_error = true;
		return false;
	}

	// if only one argument and its a directory
	if (has_one_arg)
	{
		ftls_file_info file;
		if (!retrieve_file_info(ctx, first_arg, NULL, &file)) {
			ctx->major_error = true;
			print_access_error(ctx, first_arg);
			free(first_arg);
			return false;
		}
		t_bool is_dir = file.is_dir;
		free_file_info(&file);
		if (is_dir)
		{
			ftls_dir dir;
			if (!gather_directory(ctx, first_arg, &dir)) {
				ctx->major_error = true;
				print_directory_error(ctx, first_arg);
				free(first_arg);
				return false;
			}

			ftls_print_options print_options = { .show_prefix = false, .display_full = true, .force_compose = false, .recurse = 0 };
			print_directory(ctx, &dir, print_options);
			free_directory(&dir);
			free(first_arg);
			return true;
		}
		free(first_arg);
	}

	// treat as list of files and directories
	ftls_dir dir;
	gather_composed_directory(ctx, argc, argv, &dir);
	ftls_print_options print_options = { .show_prefix = false, .display_full = false, .force_compose = true, .recurse = 1 };
	print_directory(ctx, &dir, print_options);
	free_directory(&dir);
	return true;
}
