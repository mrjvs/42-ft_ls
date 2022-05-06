#include "context.h"
#include "printing.h"
#include "cli.h"
#include <time.h>

int	main(int argc, char **argv, char **envp)
{
	ftls_context	ctx;

	ctx.envp = envp;
	init_context(&ctx);
	ctx.executable = argv[0];
	if (time(&(ctx.ops.cur_time)) == (time_t)-1) {
		print_errno(&ctx);
		return 2;
	}
	handle_argv(argc, argv, &ctx);
	if (ctx.major_error)
		return 2;
	return (ctx.error ? 1 : 0);
}
