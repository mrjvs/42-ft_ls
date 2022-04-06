#ifndef FTLS_CLI_H
# define FTLS_CLI_H

#include "context.h"

int	iter_args(int args_size, char **args, int (*iter)(void *,char), void *param);
int	handle_argv(int argc, char **argv, ftls_context *ctx);

#endif
