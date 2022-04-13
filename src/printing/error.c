#include "context.h"
#include <stdio.h>

/**
 * Print file access error in correct format
*/
void	print_access_error(char *path)
{
	// TODO proper format
	perror(path);
}

/**
 * Print generic error in correct format
*/
void	print_error(ftls_context *ctx)
{
	perror(ctx->executable);
}
