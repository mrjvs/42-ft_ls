#include "bool.h"
#include "ftls_string.h"
#include "io.h"
#include "list.h"
#include "printing.h"
#include <unistd.h>

/**
 * Get width for every column, taking in account every file
*/
static void	get_sizing_for_rows(ftls_context *ctx, int *sizes, int columns, ftls_dir *dir, size_t dir_size) {
	int i = 0;
	int j = 0;
	int rows = (dir_size / columns) + (dir_size % columns != 0);
	l_list *lst = &(dir->files);
	while ((lst = get_next_list(lst))) {
		struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
		if (!should_print_file(ctx, &(entry->file)))
			continue;
		size_t len = ftls_strlen(entry->file.name);
		if (sizes[j] < (int)len)
			sizes[j] = len;
		i++;
		if (i >= rows) {
			i = 0;
			j++;
		}
	}
}

/**
 * Check if column sizes fit in terminal columns. takes in account grid formatting
 * returns true if it will fit, false if not
*/
static t_bool check_if_fits(int *sizes, int columns, int columnChars) {
	int len = 0;
	for (int i = 0; i < columns; i++)
		len += sizes[i];
	len += (columns-1) * 2; // 2 spaces inbetween items
	if (len > columnChars)
		return false;
	return true;
}

/**
 * Get the maximum columns for file list.
 * also outputs a list of sizes for each column (make sure to free)
 * returns -1 on error and the biggest possible column amount on success
*/
int	max_columns_for_files(ftls_context *ctx, ftls_dir *dir, int **sizes) {
	int currentCol = 1;
	int columns = ctx->ops.columns;

	size_t dir_size = 0;
	l_list	*lst = &(dir->files);
	while((lst = get_next_list(lst)))
		dir_size += should_print_file(ctx, &(get_list_data(lst, struct s_ftls_dir_entry)->file)) > 0;

	// showing as rows or no columns found in terminal
	if (columns == 0 || ctx->ops.show_as_rows) {
		int *cols = malloc(sizeof(int) * 1);
		if (!cols)
			return -1;
		get_sizing_for_rows(ctx, cols, 1, dir, dir_size);
		*sizes = cols;
		return 1;
	}

	while (true) {
		int *cols = malloc(sizeof(int) * currentCol);
		if (!cols)
			return -1;
		for (int i = 0; i < currentCol; i++)
			cols[i] = 0;
		get_sizing_for_rows(ctx, cols, currentCol, dir, dir_size);
		if (!check_if_fits(cols, currentCol, columns)) {
			if (currentCol == 1)
				currentCol++;
			get_sizing_for_rows(ctx, cols, currentCol-1, dir, dir_size);
			*sizes = cols;
			return currentCol-1;
		} else if (dir_size == (size_t)currentCol) {
			*sizes = cols;
			return currentCol;
		}
		free(cols);
		currentCol++;
	}
}

/**
 * Print a grid of files
*/
void	print_grid(ftls_context *ctx, ftls_dir *dir, int columns, int *sizes) {
	// TODO grid alignment is fucked
	size_t dir_size = 0;
	l_list	*lst = &(dir->files);
	while((lst = get_next_list(lst)))
		dir_size += should_print_file(ctx, &(get_list_data(lst, struct s_ftls_dir_entry)->file)) > 0;
	int rows = (dir_size / columns) + (dir_size % columns != 0);

	// every row
	for (int y = 0; y < rows; y++) {
		lst = get_next_list((&(dir->files)));
		// amount to skip
		int to_skip = y; // offset printing by Y
		size_t prev_padd = 0;
		for (int j = 0; j < columns; j++) {
			// skip certain amount
			for (int i = 0; i < to_skip && lst;) {
				struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
				lst = get_next_list(lst);
				if (!should_print_file(ctx, &(entry->file))) {
					continue;
				}
				i++;
			}
			// skip until first printable
			while (lst) {
				struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
				if (!should_print_file(ctx, &(entry->file)))
					lst = get_next_list(lst);
				else
					break;
			}

			// if its the end, break out of rows. should only happen on the ends
			if (!lst)
				break;

			// actually print
			t_bool first = j == 0;
			int amountOfSpaces = 0;
			if (!first) {
				amountOfSpaces += 2; // 2 spaces between entries
				amountOfSpaces += prev_padd;
			}
			for (int i = 0; i < amountOfSpaces; i++)
				ftls_write(STDOUT_FILENO, " ");
			struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
			print_simple_name(ctx, &(entry->file));
			prev_padd = sizes[j] - ftls_strlen(entry->file.name);
			ctx->has_printed = true;
			
			// for next column
			to_skip = rows;
		}
		ftls_write(STDOUT_FILENO, "\n");
	}
}
