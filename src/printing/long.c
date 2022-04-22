#include "bool.h"
#include "context.h"
#include "ftls_string.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>

static void	free_str_arr(char **arr) {
	for (int i = 0; arr[i]; i++)
		free(arr[i]);
	free(arr);
}

/**
 * Get a long line and populate it into `line`.
 * returns false if failed to allocate memory
*/
static t_bool	get_long_line(ftls_context *ctx, char ***line, ftls_file_info *file) {
	(void)ctx;
	char **l = malloc(sizeof(char*) * 7);
	if (!l)
		return false;
	for (int i = 0; i < 7; i++)
		l[i] = NULL;

	// gather strings
	l[0] = ftls_strdup("TEST");
	if (!l[0]) { free_str_arr(l); return false; }
	l[1] = ftls_strdup("TEST2");
	if (!l[1]) { free_str_arr(l); return false; }
	l[2] = ftls_strdup(file->name);
	if (!l[2]) { free_str_arr(l); return false; }

	*line = l;
	return true;
}

/**
 * Actually print the long lines
*/
static t_bool	print_long_lines(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops) {
	char	***lines = NULL;
	 
	// get size of dir
	size_t	dir_size = 0;
	l_list *lst = &(dir->files);
	while ((lst = get_next_list(lst)))
	{
		ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

		if ((file.is_dir && ops.force_compose) || !should_print_file(ctx, &file))
			continue;

		dir_size++;
	}

	// allocate memory
	lines = malloc(sizeof(char*) * dir_size+1);
	if (!lines) {
		return false;
	}

	// fetch strings for printing
	size_t i = 0;
	lst = &(dir->files);
	while ((lst = get_next_list(lst)))
	{
		ftls_file_info file = get_list_data(lst, struct s_ftls_dir_entry)->file;

		if ((file.is_dir && ops.force_compose) || !should_print_file(ctx, &file))
			continue;

		if (!get_long_line(ctx, lines + i, &file)) {
			for (size_t j = 0; j < i; j++) {
				free_str_arr(lines[j]);
			}
			free(lines);
			return false;
		}
		i++;
	}

	// fetch column sizes
	int column_sizes[7];
	for (int i = 0; i < 7; i++) {
		column_sizes[i] = 0;
		for (size_t j = 0; j < dir_size; j++) {
			int size = (int)ftls_strlen(lines[j][i]);
			if (column_sizes[i] < size)
				column_sizes[i] = size;
		}
	}

	// print lines & columns
	for (size_t i = 0; i < dir_size; i++) {
		for (int j = 0; lines[i][j]; j++) {
			size_t size = ftls_strlen(lines[i][j]);

			// padding
			size_t padding = 0;
			if (j != 0 && size != 0)
				padding += 2;
			if (lines[i][j+1] != 0) // if its not the end, add more padding
				padding += column_sizes[j] - size;
			for (size_t padd_i = 0; padd_i < padding; padd_i++)
				ftls_write(STDOUT_FILENO, " ");
			
			// write data
			ftls_write(STDOUT_FILENO, lines[i][j]);
			
		}
		ftls_write(STDOUT_FILENO, "\n");
	}

	// free the data used to print
	for (size_t j = 0; j < dir_size; j++) {
		free_str_arr(lines[j]);
	}
	free(lines);
	return true;
}

/**
 * Print directory in long format
*/
void			print_long_format(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops) {
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

	// print lines
	if (!print_long_lines(ctx, dir, ops)) {
		print_directory_error(ctx, dir->name);
		return;
	}
}
