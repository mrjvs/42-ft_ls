#include "bool.h"
#include "ftls_string.h"
#include "io.h"
#include "list.h"
#include "printing.h"
#include <stddef.h>
#include <unistd.h>

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

// ------------- MACOS GRID CALCULATION -------------
#ifdef MACOS_RENDER

/**
 * Get width for every column, taking in account every file
*/
static void	get_sizing_for_rows(ftls_context *ctx, int *sizes, int columns, ftls_dir *dir, ftls_print_options ops) {
	l_list *lst = &(dir->files);
	size_t largest_len = 0;
	int i = 0;
	while ((lst = get_next_list(lst))) {
		struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
		if ((entry->file.is_dir && !ops.display_full) || !should_print_file(ctx, &(entry->file)))
			continue;
		size_t len = ftls_strlen(entry->file.name);
		if (largest_len < len)
			largest_len = len;
		i++;
	}
	for (int c = 0; c < columns; c++)
		sizes[c] = largest_len;
}

static int get_cols_for_grid(size_t dir_size, int rows) {
	return (dir_size / rows) + (dir_size % rows > 0);
}

static int get_rows_for_grid(size_t dir_size, int columns) {
	// on macos render mode, this is the same result
	return get_cols_for_grid(dir_size, columns);
}

/**
 * Get the maximum columns for file list.
 * also outputs a list of sizes for each column (make sure to free)
 * returns -1 on error and the biggest possible column amount on success
*/
int	max_columns_for_files(ftls_context *ctx, ftls_dir *dir, int **sizes, ftls_print_options ops) {
	int columns = ctx->ops.columns;

	size_t dir_size = 0;
	l_list	*lst = &(dir->files);
	size_t largest_len = 0;
	while((lst = get_next_list(lst))) {
		struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
		if ((entry->file.is_dir && !ops.display_full) || !should_print_file(ctx, &(entry->file)))
			continue;
		dir_size++;
		size_t len = ftls_strlen(entry->file.name);
		if (largest_len < len)
			largest_len = len;
	}

	// showing as rows or no columns found in terminal
	if (dir_size == 0 || columns == 0 || ctx->ops.show_as_rows) {
		int *cols = malloc(sizeof(int) * 1);
		if (!cols)
			return -1;
		get_sizing_for_rows(ctx, cols, 1, dir, ops);
		*sizes = cols;
		return 1;
	}

	int amountRows = 1;
	while (true) {
		int currentCol = get_cols_for_grid(dir_size, amountRows);
		int *cols = malloc(sizeof(int) * currentCol);
		if (!cols)
			return -1;
		for (int i = 0; i < currentCol; i++)
			cols[i] = 0;
		get_sizing_for_rows(ctx, cols, currentCol, dir, ops);
		if (amountRows <= (int)dir_size && !check_if_fits(cols, currentCol, columns)) {
			// add a row and try again
			amountRows++;
			free(cols);
			continue;
		}

		// it fits, return
		*sizes = cols;
		return currentCol;
	}
}

// ------------- LINUX GRID CALCULATION -------------
#else

static int get_rows_for_grid(size_t dir_size, int columns) {
	int count = 0;
	ssize_t s = (ssize_t)dir_size;
	while (s > 0) {
		count++;
		s -= columns;
	}
	return count;
}

/**
 * Get width for every column, taking in account every file
*/
static void	get_sizing_for_rows(ftls_context *ctx, int *sizes, int columns, ftls_dir *dir, size_t dir_size, int *lowest, ftls_print_options ops) {
	int i = 0;
	int j = 0;
	int rows = get_rows_for_grid(dir_size, columns);
	l_list *lst = &(dir->files);
	int lowestColumns = 0;
	for (int c = 0; c < columns; c++)
		sizes[c] = 0;
	while ((lst = get_next_list(lst))) {
		struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
		if ((entry->file.is_dir && !ops.display_full) || !should_print_file(ctx, &(entry->file)))
			continue;
		size_t len = ftls_strlen(entry->file.name);
		if (sizes[j] < (int)len)
			sizes[j] = len;
		i++;
		if (i >= rows) {
			if (sizes[j] != 0)
				lowestColumns = j+1;
			i = 0;
			j++;
		}
	}
	if (j < columns && sizes[j] != 0)
		lowestColumns = j+1;
	if (lowest)
		*lowest = lowestColumns;
}

/**
 * Get the maximum columns for file list.
 * also outputs a list of sizes for each column (make sure to free)
 * returns -1 on error and the biggest possible column amount on success
*/
int	max_columns_for_files(ftls_context *ctx, ftls_dir *dir, int **sizes, ftls_print_options ops) {
	int currentCol = 1;
	int columns = ctx->ops.columns;

	size_t dir_size = 0;
	l_list	*lst = &(dir->files);
	while((lst = get_next_list(lst))) {
		dir_size += !(get_list_data(lst, struct s_ftls_dir_entry)->file.is_dir && !ops.display_full) && should_print_file(ctx, &(get_list_data(lst, struct s_ftls_dir_entry)->file));
	}

	// showing as rows or no columns found in terminal
	if (dir_size == 0 || columns == 0 || ctx->ops.show_as_rows) {
		int *cols = malloc(sizeof(int) * 1);
		if (!cols)
			return -1;
		get_sizing_for_rows(ctx, cols, 1, dir, dir_size, 0, ops);
		*sizes = cols;
		return 1;
	}

	while (true) {
		int *cols = malloc(sizeof(int) * currentCol);
		int lowest;
		if (!cols)
			return -1;
		for (int i = 0; i < currentCol; i++)
			cols[i] = 0;
		get_sizing_for_rows(ctx, cols, currentCol, dir, dir_size, &lowest, ops);
		if (!check_if_fits(cols, lowest, columns)) {
			if (currentCol == 1)
				currentCol++;
			get_sizing_for_rows(ctx, cols, currentCol-1, dir, dir_size, &lowest, ops);
			*sizes = cols;
			return lowest;
		} else if (dir_size == (size_t)currentCol) {
			*sizes = cols;
			return currentCol;
		}
		free(cols);
		currentCol++;
	}
}

#endif

/**
 * Print a grid of files
*/
void	print_grid(ftls_context *ctx, ftls_dir *dir, int columns, int *sizes, ftls_print_options ops) {
	size_t dir_size = 0;
	l_list	*lst = &(dir->files);
	while((lst = get_next_list(lst)))
		dir_size += !(get_list_data(lst, struct s_ftls_dir_entry)->file.is_dir && !ops.display_full) && should_print_file(ctx, &(get_list_data(lst, struct s_ftls_dir_entry)->file));
	int rows = get_rows_for_grid(dir_size, columns);

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
				if ((entry->file.is_dir && !ops.display_full) || !should_print_file(ctx, &(entry->file))) {
					continue;
				}
				i++;
			}
			// skip until first printable
			while (lst) {
				struct s_ftls_dir_entry *entry = get_list_data(lst, struct s_ftls_dir_entry);
				if ((entry->file.is_dir && !ops.display_full) || !should_print_file(ctx, &(entry->file)))
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
