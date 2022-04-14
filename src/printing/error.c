#include "context.h"
#include "ftls_string.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/**
 * Print file access error in a specific format
 * also marks context as errors
*/
void	print_path_error(ftls_context *ctx, const char *path, const char *msg)
{
	 // TODO stderr
	ctx->error = true;
	ftls_write(ctx->executable);
	ftls_write(": ");
	ftls_write(msg);
	ftls_write(" '");
	ftls_write(path);
	ftls_write("': ");
	ftls_puts(strerror(errno));
}

/**
 * Print file access error in correct format
 * also marks context as errored
*/
void	print_access_error(ftls_context *ctx, const char *path)
{
	print_path_error(ctx, path, "cannot access");
}

/**
 * Print directory open error in correct format
 * also marks context as errored
*/
void	print_directory_error(ftls_context *ctx, const char *path)
{
	print_path_error(ctx, path, "cannot open directory");
}

/**
 * Print generic error in the format
 * Also marks context as errored
*/
void	print_error(ftls_context *ctx, const char *str)
{
	ctx->error = true;
	ftls_write(ctx->executable);
	ftls_write(": ");
	ftls_puts(str);
}

/**
 * Print errno in correct format
 * Also marks context as errored
*/
void	print_errno(ftls_context *ctx)
{
	print_error(ctx, strerror(errno));
}
