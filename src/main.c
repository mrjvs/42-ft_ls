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

int	main(int argc, char **argv, char **envp)
{
	t_ftls_context	ctx;

	(void)envp;
	init_context(&ctx);
	handle_argv(argc, argv, &ctx);
	return (0);
}
