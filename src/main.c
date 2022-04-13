#include "context.h"
#include "cli.h"

int	main(int argc, char **argv, char **envp)
{
	ftls_context	ctx;

	(void)envp;
	init_context(&ctx);
	ctx.executable = argv[0];
	handle_argv(argc, argv, &ctx);
	return (0); // TODO proper exit codes: 0 good, 1 minor, 2 major
}
