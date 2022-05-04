#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "bool.h"
#include "context.h"
#include "ftls_string.h"

/**
 * Get environment variable from context, returns null if not found
*/
static char	*ftls_getenv(ftls_context *ctx, char *key) {
	for (size_t i = 0; ctx->envp[i]; i++) {
		if (ftls_strcmp(ctx->envp[i], key) == '=')
			return ctx->envp[i] + ftls_strlen(key) + 1;
	}
	return NULL;
}

/**
 * Check if string is a valid color string
*/
static t_bool	is_valid_colors(char *colors) {
	if (!colors)
		return false;
	if (ftls_strlen(colors) != 22)
		return false;
	for (int i = 0; colors[i]; i++) {
		if (ftls_strchr("abcdefghABCDEFGHx", (int)colors[i]) == NULL)
			return false;
	}
	return true;
}

/**
 * Get a color character and copy the color print string into dest
 * undefined behaviour if color char is not valid
*/
static void	set_color_from_char(char *dest, char color, t_bool background) {
	// transparent = empty string
	if (color == 'x') {
		dest[0] = '\0';
		return;
	}

	// copy  prefix
	int i = 0;
	dest[i++] = '\e';
	dest[i++] = '[';

	// figure out color codes
	char layer_char = '3';
	char color_char = '0';
	char text_type = '0';
	if (background) layer_char++;
	if (color >= 'A' && color <= 'H') {
		text_type++;
		color_char += color - 'A';
	}
	if (color >= 'a' && color <= 'h')
		color_char += color - 'a';

	// copy colors codes
	dest[i++] = text_type;
	dest[i++] = ';';
	dest[i++] = layer_char;
	dest[i++] = color_char;

	// copy suffix and termination
	dest[i++] = 'm';
	dest[i++] = '\0';
}

/**
 * Set colors from color string to context
*/
static void	set_colors_on_context(ftls_context *ctx, char *colors) {
	set_color_from_char(ctx->colors[0][0], 'x', false);
	set_color_from_char(ctx->colors[0][1], 'x', true);
	for (int i = 0; colors[i]; i++) {
		int color_index = i / 2;
		int back_or_foreground = i % 2;
		set_color_from_char(ctx->colors[color_index+1][back_or_foreground], colors[i], back_or_foreground);
	}
}

void		init_context(ftls_context *context)
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
				int columnsNum = ftls_atoi(columns);
				ops->columns = columnsNum > 0 ? columnsNum : 0;
			}
		}
	} else {
		ops->show_as_rows = 1;
	}
	if (ops->columns == 0)
		ops->show_as_rows = 1;

	// colors
	char *lscolors = ftls_getenv(context, "LSCOLORS");
	set_colors_on_context(context, is_valid_colors(lscolors) ? lscolors : "exfxcxdxbxegedabagacad");

	context->has_printed = 0;
	context->executable = NULL;
	context->major_error = false;
	context->error = false;
}
