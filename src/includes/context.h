#ifndef FTLS_CONTEXT_H
# define FTLS_CONTEXT_H

#include "bool.h"
#include <time.h>

enum ftls_sort {
	FTLS_SORT_NONE,
	FTLS_SORT_LEXICOGRAPHICAL,
	FTLS_SORT_MODIFIED,
	FTLS_SORT_ACCESSED,
};

enum ftls_date_format {
	FTLS_CREATED_TIME,
	FTLS_ACCESSED_TIME,
	FTLS_MODIFIED_TIME,
};

enum e_ftls_color {
	C_FILE = 0,
	C_DIR = 1,
	C_SYMLINK = 2,
	C_SOCKET = 3,
	C_PIPE = 4,
	C_EXEC = 5,
	C_BLOCK_S = 6,
	C_CHAR_S = 7,
	C_EXEC_SETUID = 8,
	C_EXEC_SETGID = 9,
	C_DIR_WRITABLE = 10,
	C_DIR_WRITABLE_STICKY = 11
};

typedef char						t_color_print[10]; // color print string
typedef t_color_print				t_color_print_collection[2]; // foreground and background
typedef t_color_print_collection	t_all_colors[12]; // amount of colors

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
	char	invalid;				// set if invalid option is passed in, this is the invalid option
	t_bool	show_as_rows;			// if it should show with columns or every entry one row
	t_bool	show_access_date;		// show access date instead of modified date
	t_bool	fetch_details;			// if it should fetch details
	time_t	cur_time;
	enum ftls_date_format date_format;
}	ftls_options;

typedef struct s_ftls_context {
	ftls_options	ops;
	t_bool			has_printed;
	t_bool			major_error;
	t_bool			error;
	t_all_colors	colors;			// colors used for printing
	char			**envp;			// pointer to environment variables
	char			*executable;	// name of executable thats being used
}	ftls_context;

/**
 * Initialize a FTLS context.
 * this is used throughout the entire program and stores all options
 * that are passed in
*/
void	init_context(ftls_context *context);

#endif
