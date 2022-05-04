#include "ftls_string.h"
#include "io.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Duplicate string
 * Do not pass NULL
 *
 * Returns allocated string
*/
char	*ftls_strdup(const char *str)
{
	char *out = malloc(ftls_strlen(str) + 1);
	size_t i = 0;
	for (; str[i]; i++)
		out[i] = str[i];
	out[i] = 0;
	return out;
}

/**
 * Get length of string
 * returns length of string, 0 if NULL
*/
size_t	ftls_strlen(const char *str)
{
	if (!str)
		return 0;

	size_t i = 0;
	for (; str[i]; i++);
	return i;
}

/**
 * sets N bytes to 0 in data
*/
void	ftls_bzero(void *data, size_t n)
{
	for (size_t i = n; i; i--)
		((char*)data)[i-1] = 0;
}

/**
 * Allocates N bytes of specified size. also NULL's the memory
 * returns pointer to data or NULL on failure
*/
void	*ftls_calloc(size_t n, size_t size)
{
	void	*data = malloc(n * size);
	if (data)
		ftls_bzero(data, n * size);
	return data;
}

/**
 * Appends SRC to the end of DEST
 * Memory should be big enough
 *
 * returns dest
*/
char	*ftls_strcat(char *dest, const char *src)
{
	char *append = dest + ftls_strlen(dest);
	size_t i;
	for (i = 0; src[i]; i++)
		append[i] = src[i];
	append[i] = 0;
	return dest;
}

/**
 * Compares 2 strings
 *
 * returns the difference, 0 means equal
*/
int		ftls_strcmp(const char *one, const char *two)
{
	size_t i = 0;
	while (1) {
		if (one[i] != two[i])
			return one[i] - two[i];
		if (one[i] == '\0')
			return 0;
		i++;
	}
}

/**
 * Write a string to fd
*/
ssize_t	ftls_write(int fd, const char *str)
{
	return write(fd, str, ftls_strlen(str));
}

/**
 * Write a string to fd and add a newline
*/
ssize_t	ftls_puts(int fd, const char *str)
{
	ssize_t out = ftls_write(fd, str);
	if (out == -1)
		return -1;
	if (ftls_write(fd, "\n") == -1)
		return -1;
	return out + 1;
}

char	*ftls_strchr(char *str, char c) {
	while (*str != 0) {
		if (*str == c)
			return str;
		str++;
	}
	return NULL;
}
