#include <stdio.h>
#include "context.h"
#include "cli.h"
#include "io.h"

static int	process_option(void *ctx_ptr, char option)
{
	ftls_context *ctx  = ctx_ptr;

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
	else
		return -1;
	return 0;
}

static void	post_process_options(ftls_context *ctx)
{
	if (!ctx->ops.should_sort)
		ctx->ops.sort_method = FTLS_SORT_LEXICOGRAPHICAL;
	if (ctx->ops.disable_sort)
		ctx->ops.sort_method = FTLS_SORT_NONE;
}

static void process_invalid_args(int err)
{
	// TODO better error handling
	if (err == -1)
		puts("Unknown option passed in.");
	else
		puts("Unhandled argument parsing error");
}

t_bool		handle_argv(int argc, char **argv, ftls_context *ctx)
{
	int argv_pos = 1;

	// parse arguments
	int args_ret = iter_args(argc - argv_pos, argv + argv_pos, process_option, ctx);
	argv_pos += args_ret;

	// handle errors for argument parsing
	if (args_ret < 0)
	{
		process_invalid_args(args_ret);
		return false;
	}

	// post processing of arguments
	post_process_options(ctx);

	// run gathering and printing section
	gather_and_print(ctx, argc - argv_pos, argv+argv_pos);
	return true;
}
