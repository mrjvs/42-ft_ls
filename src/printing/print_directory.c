#include <stdio.h>
#include <string.h>
#include "context.h"
#include "io.h"
#include "printing.h"

/**
 * print directories, in case of recursive. gather and then print after printing main deal
*/
void	print_directory(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops) {
	if (ctx->ops.recursive)
		ops.recurse = -1;

	// TODO temp printf
	if (ops.show_prefix)
		printf("%s:\n", dir->name);

	// print standard
	l_list *lst = &(dir->files);
	while ((lst = get_next_list(lst)))
	{
		ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

		if ((file.is_dir && ops.force_compose) || !should_print_file(ctx, file))
			continue;

		ctx->has_printed = true;
		printf("DIR: %i, %s === %s\n", file.is_dir, file.path, file.name);
	}

	if (ops.recurse == 0)
		return;

	// recurse directories
	ops.show_prefix = true;
	t_bool force_compose = ops.force_compose;
	ops.force_compose = false;
	if (ops.recurse > 0)
		ops.recurse--;
	lst = &(dir->files);
	while ((lst = get_next_list(lst)))
	{
		ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

		// only handle files && non-relative directories (unless forced) && it should only print hidden files if forced
		if (!file.is_dir || (file.is_relative && !force_compose) || (!force_compose && !should_print_file(ctx, file)))
			continue;

		// gather and print directory
		ftls_dir dir;
		if (!gather_directory(ctx, file.path, &dir)) {
			if (force_compose)
				ctx->major_error = true; // its only a major error if it fails on recurse
			continue;
		}
		if (ctx->has_printed)
			printf("\n");
		ctx->has_printed = true;
		print_directory(ctx, &dir, ops);
		free_directory(&dir);
	}
}
