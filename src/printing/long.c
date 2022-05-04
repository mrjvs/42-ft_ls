#include "bool.h"
#include "context.h"
#include "ftls_string.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>
#include <unistd.h>

static void	free_str_arr(struct s_ftls_col *arr) {
	for (int i = 0; arr[i].str; i++)
		free(arr[i].str);
	free(arr);
}

// TODO timestamps
// TODO options
/**
 * Get a long line and populate it into `line`.
 * returns false if failed to allocate memory
*/
static t_bool	get_long_line(ftls_context *ctx, struct s_ftls_col **line, ftls_file_info *file) {
	(void)ctx;
	struct s_ftls_col *l = malloc(sizeof(struct s_ftls_col) * 7);
	if (!l)
		return false;
	for (int i = 0; i < 7; i++) {
		l[i].name = false;
		l[i].str = NULL;
		l[i].exists = false;
		l[i].right_align = false;
	}

	// gather strings
	l[0].str = get_perms(file);
	if (!l[0].str) { free_str_arr(l); return false; }
	l[0].exists = true;

	l[1].str = ftls_ltoa(file->stat.st_nlink);
	if (!l[1].str) { free_str_arr(l); return false; }
	l[1].right_align = true;
	l[1].exists = true;

	l[2].str = ftls_strdup(file->user);
	if (!l[2].str) { free_str_arr(l); return false; }
	l[2].exists = true;

	l[3].str = ftls_strdup(file->group);
	if (!l[3].str) { free_str_arr(l); return false; }
	l[3].exists = true;

	l[4].str = ftls_ltoa(file->stat.st_size);
	if (!l[4].str) { free_str_arr(l); return false; }
	l[4].right_align = true;
	l[4].exists = true;

	l[5].str = NULL;
	l[5].name = true;
	l[5].file = *file;
	l[5].exists = true;

	*line = l;
	return true;
}

/**
 * Actually print the long lines
*/
static t_bool	print_long_lines(ftls_context *ctx, ftls_dir *dir, ftls_print_options ops) {
	struct s_ftls_col	**lines = NULL;
	 
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
	lines = malloc(sizeof(struct s_ftls_col *) * dir_size+1);
	if (!lines) {
		return false;
	}
	lines[dir_size] = NULL;

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
			int size = (int)ftls_strlen(lines[j][i].str);
			if (column_sizes[i] < size)
				column_sizes[i] = size;
		}
	}

	// print lines & columns
	for (size_t i = 0; i < dir_size; i++) {
		size_t padding_after = 0;
		for (int j = 0; lines[i][j].exists; j++) {
			size_t size = ftls_strlen(lines[i][j].str);

			// padding
			size_t padding = 0;
			if (j != 0)
				padding += 1;
			if (lines[i][j].right_align)
				padding += column_sizes[j] - size;
			padding += padding_after;
			for (size_t padd_i = 0; padd_i < padding; padd_i++)
				ftls_write(STDOUT_FILENO, " ");
			
			// write data
			if (lines[i][j].name) {
				print_simple_name(ctx, &(lines[i][j].file));
				if (lines[i][j].file.is_link) {
					ftls_write(STDOUT_FILENO, " -> ");
					ftls_write(STDOUT_FILENO, lines[i][j].file.link_path);
				}
			}
			else
				ftls_write(STDOUT_FILENO, lines[i][j].str);
			
			padding_after = 0;
			if (!lines[i][j].right_align)
				padding_after += column_sizes[j] - size;
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
		ftls_write(STDOUT_FILENO, "total ");
		char *block_str = ftls_ltoa(blocks);
		if (block_str) {
			ftls_write(STDOUT_FILENO, block_str);
			free(block_str);
		}
		ftls_write(STDOUT_FILENO, "\n");

	}

	// print lines
	if (!print_long_lines(ctx, dir, ops)) {
		print_directory_error(ctx, dir->name);
		return;
	}
}
