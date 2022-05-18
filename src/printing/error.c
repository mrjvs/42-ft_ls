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
	ctx->error = true;
	int fd = STDERR_FILENO;
	ftls_write(fd, ctx->executable);
	ftls_write(fd, ": ");
#ifdef MACOS_RENDER
	(void)msg;
	ftls_write(fd, path);
	ftls_write(fd, ": ");
#else
	ftls_write(fd, msg);
	ftls_write(fd, " '");
	ftls_write(fd, path);
	ftls_write(fd, "': ");
#endif
	ftls_puts(fd, strerror(errno));
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
	int fd = STDERR_FILENO;
	ftls_write(fd, ctx->executable);
	ftls_write(fd, ": ");
	ftls_puts(fd, str);
}

/**
 * Print errno in correct format
 * Also marks context as errored
*/
void	print_errno(ftls_context *ctx)
{
	print_error(ctx, strerror(errno));
}
