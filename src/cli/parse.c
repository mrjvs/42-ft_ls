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

#include "context.h"
#include "cli.h"
#include <stdio.h>

static void	process_option(void *ctx_ptr, char option)
{
	t_ftls_context *ctx;
	
	ctx = ctx_ptr;
	if (option == 'l')
		ctx->ops.show_long = 1;
	else if (option == 'r')
		ctx->ops.should_reverse = 1;
	else if (option == 'a')
		ctx->ops.list_all = 1;
	else if (option == 'R')
		ctx->ops.recursive = 1;
	else if (option == 't')
		ctx->ops.should_sort = 1;
	else if (option == 't')
		ctx->ops.should_sort = 1;
	else if (option == 'u')
		ctx->ops.sort_method = FTLS_SORT_ACCESSED;
	else if (option == 'f')
		ctx->ops.disable_sort = 1;
	else if (option == 'g')
		ctx->ops.only_show_group = 1;
	else if (option == 'd')
		ctx->ops.dir_as_file = 1;
}

static void	post_process_options(t_ftls_context *ctx)
{
	if (!ctx->ops.should_sort)
		ctx->ops.sort_method = FTLS_SORT_LEXICOGRAPHICAL;
	if (ctx->ops.disable_sort)
		ctx->ops.sort_method = FTLS_SORT_NONE;
}

int			handle_argv(int argc, char **argv, t_ftls_context *ctx)
{
	int argv_pos;

	argv_pos = 1;
	argv_pos += iter_args(argc-argv_pos, argv+argv_pos, process_option, ctx);
	post_process_options(ctx);
	while (argv_pos < argc)
	{
		puts(argv[argv_pos]); // TODO temp
		argv_pos++;
	}
	return 1;
}
