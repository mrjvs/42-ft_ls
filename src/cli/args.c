#include <stddef.h>

/**
 * iterate over input arguments
 * the 'iter' function takes the 'param' parameter as argument
 * and the character of the option
 * The 'iter' function can return a negative int, that indicates an error code and
 * will be passed down to the original caller
 * 
 * returns amount of arguments iterated over, negative if error
 */
int	iter_args(int args_size, char **args, int (*iter)(void *,char), void *param)
{
	int i = 0;
	for (; i < args_size; i++)
	{
		if (args[i][0] != '-' || args[i][1] == '\0')
			break;
		for (int j = 1; args[i][j] != '\0'; j++)
		{
			int err = iter(param, args[i][j]);
			if (err < 0)
				return err;
		}
	}

	return i;
}
