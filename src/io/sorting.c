#include "context.h"
#include "ftls_string.h"
#include "io.h"
#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>

static int	compare_timespec(struct timespec a, struct timespec b) {
	if (a.tv_sec > b.tv_sec)
		return -1;
	if (a.tv_sec < b.tv_sec)
		return 1;
	if (a.tv_nsec > b.tv_nsec)
		return -1;
	if (a.tv_nsec < b.tv_nsec)
		return 1;
	return 0;
}

/**
 * simplified version of strcoll, only does alphabet and numbers
*/
static int	ftls_strcoll(char *a, char *b)
{
	static const char order[] = ".0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";
	int ind_a = -1;
	int ind_b = -1;
	int i = 0;

	// loop until difference
	while (a[i] == b[i] && a[i])
		i++;
	if (!a[i] && !b[i])
		return 0; // no difference
	
	// find difference in (custom janky) collating order
	char char_a = a[i];
	char char_b = b[i];
	for (i = 0; order[i]; i++) {
		if (char_a == order[i])
			ind_a = i;
		if (char_b == order[i])
			ind_b = i;
		if (ind_b >= 0 && ind_a >= 0)
			break;
	}

	if (ind_a == -1 && ind_b == -1)
		return char_a - char_b; // standard number compare if both unknown symbols
	// unknown symbols always get put on bottom
	if (ind_b == -1)
		return 1;
	if (ind_a == -1)
		return -1;
	return ind_a - ind_b;
}

/**
 * sorting method. return greater than 0 if they are not in the right order
*/
static int	compare_files(ftls_context *ctx, ftls_file_info *a, ftls_file_info *b)
{
	// skip sort
	if (ctx->ops.sort_method == FTLS_SORT_NONE)
		return 0;

	// priority sorting (relative directories)
	if (a->has_sorting_priority || b->has_sorting_priority) {
		if (a->has_sorting_priority && !b->has_sorting_priority)
			return -1;
		if (b->has_sorting_priority && !a->has_sorting_priority)
			return 1;
		// TODO default to alphabetical sort
		return ftls_strcoll(a->name, b->name);
	}
	
	if (ctx->ops.sort_method == FTLS_SORT_ACCESSED)
		return compare_timespec(a->stat.st_atim, b->stat.st_atim);
	if (ctx->ops.sort_method == FTLS_SORT_MODIFIED)
		return compare_timespec(a->stat.st_mtim, b->stat.st_mtim);
	// alphabetical sort (default)
	return ftls_strcoll(a->name, b->name);
}

/**
 * Sort the files of a directory based on the context options
*/
void	sort_directory(ftls_context *ctx, ftls_dir *dir)
{
	l_list *prev = dir->files.next;
	if (prev == NULL || prev->next == NULL)
		return;

	// get len
	size_t len = 0;
	l_list *curr = prev;
	while (curr) {
		len++;
		curr = curr->next;
	}

	// bubble sort
	for (size_t it = 0; it < len; it++) {
		l_list *prev_back = &(dir->files);
		prev = prev_back->next;
		curr = prev->next;
		while (curr) {
			if (compare_files(ctx, &(get_list_data(prev, struct s_ftls_dir_entry)->file), &(get_list_data(curr, struct s_ftls_dir_entry)->file)) > 0) {
				// swap pointers
				prev_back->next = curr;
				prev->next = curr->next;
				curr->next = prev;

				// swap variables (so loop works again)
				l_list *tmp = curr;
				curr = prev;
				prev = tmp;
			}
			prev_back = prev_back->next;
			prev = prev->next;
			curr = curr->next;
		}
	}

	// reverse sort
	if (ctx->ops.should_reverse) {
		l_list *curr = dir->files.next;
		l_list *prev = NULL;
		while (curr) {
			l_list *next = curr->next;
			curr->next = prev;

			prev = curr;
			curr = next;
		}
		dir->files.next = prev;
	}
}
