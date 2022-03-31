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

#ifndef CLI_H
# define CLI_H

#include "context.h"

int	iter_args(int args_size, char **args, void (*iter)(void *,char), void *param);
int	handle_argv(int argc, char **argv, t_ftls_context *ctx);

#endif
