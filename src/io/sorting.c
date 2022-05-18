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
 * sorting method. return greater than 0 if they are not in the right order
*/
static int	compare_files(ftls_context *ctx, ftls_file_info *a, ftls_file_info *b)
{
	// skip sort
	if (ctx->ops.sort_method == FTLS_SORT_NONE)
		return 0;

	if (ctx->ops.sort_method == FTLS_SORT_ACCESSED)
		return compare_timespec(a->stat.st_atim, b->stat.st_atim);
	if (ctx->ops.sort_method == FTLS_SORT_MODIFIED)
		return compare_timespec(a->stat.st_mtim, b->stat.st_mtim);
	// ascii sort (default)
	return ftls_strcmp(a->name, b->name);
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
