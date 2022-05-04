#ifndef FTLS_STRING
#define FTLS_STRING

#include <stddef.h>
#include <unistd.h>

char	*ftls_strdup(const char *str);
size_t	ftls_strlen(const char *str);
char	*ftls_strcat(char *dest, const char *src);
int		ftls_strcmp(const char *one, const char *two);

ssize_t	ftls_puts(int fd, const char *str);
ssize_t	ftls_write(int fd, const char *str);

void	*ftls_calloc(size_t n, size_t size);
void	ftls_bzero(void *data, size_t n);

char	*ftls_ltoa(long n);

#endif
