#include "context.h"
#include "cli.h"

int	main(int argc, char **argv, char **envp)
{
	ftls_context	ctx;

	(void)envp;
	init_context(&ctx);
	ctx.executable = argv[0];
	handle_argv(argc, argv, &ctx);
	if (ctx.major_error)
		return 2;
	return (ctx.error ? 1 : 0);
}
