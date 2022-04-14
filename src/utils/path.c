#include <stddef.h>
#include <stdlib.h>
#include "ftls_string.h"

/**
 * Join two linux paths, will account for seperators.
 * Does not resolve relative segments
 * 
 * returns allocated string containing joined strings
*/
char	*path_join(char *first, char *second)
{
	size_t	first_len = ftls_strlen(first);
	char	first_has_sep = first_len > 0 && first[first_len-1] == '/';

	size_t	second_len = ftls_strlen(second);
	char	second_has_sep = second_len > 0 && second[1] == '/';

	// build string
	char	*output = calloc(first_len + second_len + 2, 1); // two paths + seperator + NULL
	ftls_strcat(output, first);
	if (!first_has_sep)
		ftls_strcat(output, "/");
	ftls_strcat(output, second + second_has_sep); // skip seperator if it has one

	return output;
}
