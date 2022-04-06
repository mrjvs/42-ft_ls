#ifndef FTLS_CONTEXT_H
# define FTLS_CONTEXT_H

#include "bool.h"

enum ftls_sort {
	FTLS_SORT_NONE,
	FTLS_SORT_LEXICOGRAPHICAL,
	FTLS_SORT_MODIFIED,
	FTLS_SORT_ACCESSED,
};

/**
 * ftls options struct, keeps track of options for the runtime.
*/
typedef struct s_ftls_options {
	t_bool	show_long;				// if it should display in detailed list form
	t_bool	show_colors;			// if it should display with colors
	t_bool	list_all;				// if it should list directories starting with a dot (.)
	t_bool	follow_links;			// if it should follow symbolic links to target instead of listing the link itself
	t_bool	recursive;				// if it should scan directories recursively and list them
	t_bool	disable_sort;			// if sorting is disabled (overwrites should_sort)
	t_bool	should_sort;			// if sorting is enabled
	enum ftls_sort	sort_method;	// how the output should be sorted (use this for checking the sorting)
	t_bool	should_reverse;			// if it should reverse the sorting
	t_bool	only_show_group;		// if it should ONLY show group (and not owner) in long list format
	t_bool	dir_as_file;			// if it should treat a dir input argument as a file
	int		columns; 				// how wide the terminal is
}	ftls_options;

typedef struct s_ftls_context {
	ftls_options	ops;
}	ftls_context;

/**
 * Initialize a FTLS context.
 * this is used throughout the entire program and stores all options
 * that are passed in
*/
void	init_context(ftls_context *context);

#endif
