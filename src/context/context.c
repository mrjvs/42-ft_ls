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

void	init_context(t_ftls_context *context)
{
	t_ftls_options	*ops;

	ops = &context->ops;
	ops->show_long = 1;
	ops->columns = 25; // TODO temp
	ops->dir_as_file = 0;
	ops->follow_links = 0;
	ops->list_all = 0;
	ops->only_show_group = 0;
	ops->recursive = 0;
	ops->should_reverse = 0;
	ops->show_colors = 0;
	ops->disable_sort = 0;
	ops->should_sort = 0;
	ops->sort_method = FTLS_SORT_MODIFIED;
}
