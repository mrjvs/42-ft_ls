#include "context.h"
#include <stdio.h>

/**
 * Print file access error in correct format
 * also marks context as errors
*/
void	print_access_error(ftls_context *ctx, char *path)
{
	// TODO proper format
	ctx->error = true;
	perror(path);
}

/**
 * Print generic error in correct format
 * Also marks context as errored
*/
void	print_error(ftls_context *ctx)
{
	ctx->error = true;
	perror(ctx->executable);
}
