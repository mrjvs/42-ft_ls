#include "bool.h"
#include "ftls_string.h"
#include "io.h"
#include "list.h"

/**
 * Get width for every column, taking in account every file
*/
static void	get_sizing_for_rows(int *sizes, int columns, ftls_dir *dir) {
	int i = 0;
	int j = 0;
	l_list *lst = &(dir->files);
	while ((lst = get_next_list(lst))) {
		size_t len = ftls_strlen(get_list_data(lst, struct s_ftls_dir_entry)->file.name);
		if (sizes[j] < (int)len)
			sizes[j] = len;
		i++;
		if (i >= columns) {
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
	if (len < columnChars)
		return false;
	return true;
}

/**
 * Get the maximum columns for file list.
 * also outputs a list of sizes for each column (make sure to free)
 * returns -1 on error and the biggest possible column amount on success
*/
int	max_columns_for_files(int columns, ftls_dir *dir, int **sizes) {
	int currentCol = 1;
	while (true) {
		int *cols = malloc(sizeof(int) * columns);
		if (!cols)
			return -1;
		get_sizing_for_rows(cols, currentCol, dir);
		if (!check_if_fits(cols, currentCol, columns)) {
			get_sizing_for_rows(cols, currentCol-1, dir);
			*sizes = cols;
			return currentCol-1;
		}
		free(cols);
		currentCol++;
	}
}
