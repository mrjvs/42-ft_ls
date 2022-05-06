#include <stdio.h>
#include "context.h"
#include "cli.h"
#include "ftls_string.h"
#include "io.h"
#include "printing.h"

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
	else if (option == 'u')
		ctx->ops.sort_method = FTLS_SORT_ACCESSED;
	else if (option == 'f')
		ctx->ops.disable_sort = 1;
	else if (option == 'g')
		ctx->ops.only_show_group = 1;
	else if (option == 'd')
		ctx->ops.dir_as_file = 1;
	else
	{
		ctx->ops.invalid = option;
		return -1;
	}
	return 0;
}

static void	post_process_options(ftls_context *ctx)
{
	if (ctx->ops.show_long && ctx->ops.sort_method == FTLS_SORT_ACCESSED)
		ctx->ops.show_access_date = true;
	if (!ctx->ops.should_sort)
		ctx->ops.sort_method = FTLS_SORT_LEXICOGRAPHICAL;
	if (ctx->ops.disable_sort) {
		ctx->ops.sort_method = FTLS_SORT_NONE;
		ctx->ops.list_all = true;
	}
	if (ctx->ops.show_long)
		ctx->ops.fetch_details = true;
}

static void process_invalid_args(ftls_context *ctx, int err)
{
	if (err == -1)
	{
		char *str = ftls_strdup("invalid option -- 'X'");
		if (!str) {
			print_errno(ctx);
			return;
		}
		str[19] = ctx->ops.invalid;
		print_error(ctx, str);
		free(str);
		return;
	}
	
	print_error(ctx, "unhandled error occured while parsing arguments");
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
		process_invalid_args(ctx, args_ret);
		ctx->major_error = true;
		return false;
	}

	// post processing of arguments
	post_process_options(ctx);

	// run gathering and printing section
	gather_and_print(ctx, argc - argv_pos, argv+argv_pos);
	return true;
}
