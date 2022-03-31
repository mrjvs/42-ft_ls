/* ************************************************************************** */
/*                                                                            */
/*   Cats are the best!                                          _ |\_        */
/*   And forced headers are annoying                             \` ..\       */
/*                                                          __,.-" =__Y=      */
/*   By: the cats                                         ."        )         */
/*                                                  _    /   ,    \/\_        */
/*   Created: // :: by                             ((____|    )_-\ \_-`       */
/*   Updated: // :: by                             `-----'`-----` `--`        */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

enum e_ftls_sort {
	FTLS_SORT_NONE,
	FTLS_SORT_LEXICOGRAPHICAL,
	FTLS_SORT_MODIFIED,
	FTLS_SORT_ACCESSED,
};

/*
** ftls options struct, keeps track of options for the runtime.
** show_long		if it should display in detailed list form
** show_colors		if it should display with colors
** list_all			if it should list directories starting with a dot (.)
** follows_links	if it should follow symbolic links to target instead
**					of listing the link itself
** recursive		if it should scan directories recursively and list them
** should_sort		if sorting is enabled
** disable_sort		if sorting is disabled (overwrites should_sort)
** sort_method		how the output should be sorted (use this for checking the sorting)
** should_reverse	if it should reverse the sorting
** only_show_group	if it should ONLY show group in long list format
** dir_as_file		if it should treat a dir input argument as a file
** columns			how wide the terminal is
*/

typedef struct s_ftls_options {
	char				show_long;
	char				show_colors;
	char				list_all;
	char				follow_links;
	char				recursive;
	char				disable_sort;
	char				should_sort;
	enum e_ftls_sort	sort_method;
	char				should_reverse;
	char				only_show_group;
	char				dir_as_file;
	int					columns;
}	t_ftls_options;

typedef struct s_ftls_context {
	t_ftls_options	ops;
}	t_ftls_context;

/*
** Initialize a FTLS context.
** this is used throughout the entire program and stores all options
** that are passed in
*/

void	init_context(t_ftls_context *context);

#endif
