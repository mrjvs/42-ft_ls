#include "context.h"
#include "cli.h"

int	main(int argc, char **argv, char **envp)
{
	ftls_context	ctx;

	(void)envp;
	init_context(&ctx);
	handle_argv(argc, argv, &ctx);
	return (0);
}
