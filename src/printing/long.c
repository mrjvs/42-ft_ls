#include "bool.h"
#include "context.h"
#include "ftls_string.h"
#include "io.h"
#include "printing.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static void	free_str_arr(struct s_ftls_col *arr) {
	for (int i = 0; arr[i].str; i++)
		free(arr[i].str);
	free(arr);
}

static char	*get_column_from_string(char *str, int col) {
	int start = 0;
	int col_count = 0;
	int i = 0;
	for (; str[i]; i++) {
		if (str[i] == ' ') {
			if (col_count == col) {
				break;
			}
			while (str[i+1] == ' ')
				i++;
			start = i+1;
			col_count++;
		}
	}
	if (col_count != col) {
		return NULL;
	}
	char tmp = str[i];
	str[i] = '\0';
	char *ret = ftls_strdup(str+start);
	str[i] = tmp;
	return ret;
}

static t_bool	extract_timestamp(ftls_context *ctx, time_t tim, struct s_ftls_col *lines, int i, char *timestr) {
	lines[i].str = get_column_from_string(timestr, 1);
	lines[i].exists = true;
	if (!lines[i].str) {
		return false;
	}
	lines[i+1].str = get_column_from_string(timestr, 2);
	lines[i+1].exists = true;
	lines[i+1].right_align = true;
	if (!lines[i+1].str) {
		return false;
	}
	// show year
	if (ctx->ops.cur_time - tim >= FTLS_TIME_TO_SHOW_YEAR) {
		lines[i+2].str = get_column_from_string(timestr, 4);
		lines[i+2].exists = true;
		lines[i+2].right_align = true;
		if (!lines[i+2].str) {
			return false;
		}
		lines[i+2].str[ftls_strlen(lines[i+2].str)-1] = '\0';
	}
	// else show time
	else {
		lines[i+2].str = get_column_from_string(timestr, 3);
		lines[i+2].exists = true;
		lines[i+2].right_align = true;
		if (!lines[i+2].str) {
			return false;
		}
		for (int ind = 0, j = 0; lines[i+2].str[ind]; ind++) {
			if (lines[i+2].str[ind] == ':') {
				j++;
				if (j == 2) {
					lines[i+2].str[ind] = '\0';
					break;
				}
			}
		}
	}
	return true;
}

// TODO options
/**
 * Get a long line and populate it into `line`.
 * returns false if failed to allocate memory
*/
static t_bool	get_long_line(ftls_context *ctx, struct s_ftls_col **line, ftls_file_info *file) {
	struct s_ftls_col *l = malloc(sizeof(struct s_ftls_col) * 10);
	if (!l)
		return false;
	for (int i = 0; i < 10; i++) {
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

	l[2].str = ftls_strdup(ctx->ops.only_show_group ? "" : file->user);
	if (!l[2].str) { free_str_arr(l); return false; }
	l[2].exists = true;

	l[3].str = ftls_strdup(file->group);
	if (!l[3].str) { free_str_arr(l); return false; }
	l[3].exists = true;

	l[4].str = ftls_ltoa(file->stat.st_size);
	if (!l[4].str) { free_str_arr(l); return false; }
	l[4].right_align = true;
	l[4].exists = true;

	time_t time = file->stat.st_mtim.tv_sec;
	char *timestr = ctime(&time);
	if (!extract_timestamp(ctx, time, l, 5, timestr)) {
		free_str_arr(l);
		return false;
	}

	l[8].str = NULL;
	l[8].name = true;
	l[8].file = *file;
	l[8].exists = true;

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

		if ((file.is_dir && !ops.display_full) || !should_print_file(ctx, &file))
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

		if ((file.is_dir && !ops.display_full) || !should_print_file(ctx, &file))
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
	int column_sizes[10];
	for (int i = 0; i < 10; i++) {
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
			if (j != 0 && (column_sizes[j] > 0 || lines[i][j].name))
				padding += 1;
			if (lines[i][j].right_align)
				padding += column_sizes[j] - size;
			padding += padding_after;
			for (size_t padd_i = 0; padd_i < padding; padd_i++)
				ftls_write(STDOUT_FILENO, " ");
			
			// write data
			if (lines[i][j].name) {
				ftls_file_info *f = &(lines[i][j].file);
				print_simple_name(ctx, f);
				if (f->is_link) {
					ftls_write(STDOUT_FILENO, " -> ");
					ftls_write(STDOUT_FILENO, f->link_path);
				}
			}
			else
				ftls_write(STDOUT_FILENO, lines[i][j].str);
			ctx->has_printed = true;
			
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
	if (ops.show_total) {
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
		ctx->has_printed = true;
	}

	// print lines
	if (!print_long_lines(ctx, dir, ops)) {
		print_directory_error(ctx, dir->name);
		return;
	}
}
