#include "context.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>

/**
 * Print directory in long format
*/
void	print_long_format(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops) {
	// show blocks if not composed
	if (!ops.force_compose) {
		size_t blocks = 0;
		l_list *lst = &(dir->files);
		while ((lst = get_next_list(lst)))
		{
			ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

			if (should_print_file(ctx, &file))
				blocks += file.stat.st_blocks / 2; // stat gives 512 blocks, we need to display 1024 blocks
		}
		printf("total %li\n", blocks); // TODO remove printf
	}

	// print long format lines
	l_list *lst = &(dir->files);
	while ((lst = get_next_list(lst)))
	{
		ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

		if ((file.is_dir && ops.force_compose) || !should_print_file(ctx, &file))
			continue;

		ctx->has_printed = true; // TODO remove printf
		printf("DIR: %i, %s === %s\n", file.is_dir, file.path, file.name);
	}
}
