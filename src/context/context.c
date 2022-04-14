#include <stddef.h>
#include "context.h"

void	init_context(ftls_context *context)
{
	ftls_options	*ops = &context->ops;
	ops->show_long = 0;
	ops->columns = 25; // TODO temp
	ops->dir_as_file = 0;
	ops->follow_links = 0;
	ops->list_all = 0;
	ops->only_show_group = 0;
	ops->recursive = 0;
	ops->should_reverse = 0;
	ops->show_colors = 0;
	ops->disable_sort = 0;
	ops->should_sort = 0;
	ops->sort_method = FTLS_SORT_MODIFIED;

	context->has_printed = 0;
	context->executable = NULL;
	context->major_error = false;
	context->error = false;
}
