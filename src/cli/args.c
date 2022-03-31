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

#include <stddef.h>

/*
** iterate over input arguments
** the 'iter' function takes the 'param' parameter as argument
** and the character of the option
** 
** returns amount of arguments iterated over 
*/

int	iter_args(int args_size, char **args, void (*iter)(void *,char), void *param)
{
	int	i;
	int j;

	i = 0;
	while (i < args_size)
	{
		if (args[i][0] != '-' || args[i][1] == '\0')
			break;
		j = 1;
		while (args[i][j] != '\0')
		{
			iter(param, args[i][j]);
			j++;
		}
		i++;
	}

	return i;
}
