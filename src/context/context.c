#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "context.h"
#include "ftls_string.h"

static char	*ftls_getenv(ftls_context *ctx, char *key) {
	for (size_t i = 0; ctx->envp[i]; i++) {
		if (ftls_strcmp(ctx->envp[i], key) == '=')
			return ctx->envp[i] + ftls_strlen(key) + 1;
	}
	return NULL;
}

void	init_context(ftls_context *context)
{
	ftls_options	*ops = &context->ops;
	ops->show_long = 0;
	ops->columns = 0;
	ops->show_as_rows = 0;
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
	ops->show_access_date = 0;
	ops->fetch_details = 0;

	// get columns or env from terminal if it is one, otherwise turn on columns mode
	// if anything fails, or columns is zero. also turn on columns mode
	struct winsize w;
	if (isatty(STDOUT_FILENO)) {
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
			ops->columns = w.ws_col;
		else {
			char *columns = ftls_getenv(context, "COLUMNS");
			if (columns) {
				int columnsNum = atoi(columns); // TODO remove atoi
				ops->columns = columnsNum > 0 ? columnsNum : 0;
			}
		}
	} else {
		ops->show_as_rows = 1;
	}
	if (ops->columns == 0)
		ops->show_as_rows = 1;

	char *lscolors = ftls_getenv(context, "LSCOLORS");
	if (lscolors)
		context->colors = lscolors;

	context->has_printed = 0;
	context->executable = NULL;
	context->major_error = false;
	context->error = false;
}
