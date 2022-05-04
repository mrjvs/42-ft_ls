#include "bool.h"
#include <stdlib.h>

char	*ftls_ltoa(long n) {
	// allocate
	char *str = malloc(21);
	int start = 0;
	if (!str)
		return NULL;

	// minus sign
	t_bool is_neg = n < 0;
	if (is_neg) {
		str[0] = '-';
		start++;
	}

	// write mirrored output to string
	int i = start;
	do {
		int chr = n % 10;
		if (chr < 0) chr *= -1;
		str[i] = chr + '0';
		n = n / (is_neg ? -10 : 10);
		is_neg = false;
		i++;
	} while (n != 0);
	str[i] = 0;

	// flip output
	for (int j = 0; j < (i / 2); j++) {
		char tmp = str[i-j-1];
		str[i-j-1] = str[j+start];
		str[j+start] = tmp;
	}

	return str;
}

int	ftls_atoi(char *str) {
	if (!str)
		return 0;
	int out = 0;
	int sign = 1;
	if (*str == '-') {
		str++;
		sign = -1;
	}
	while (*str >= '0' && *str <= '9') {
		out *= 10;
		out += *str - '0';
		str++;
	}
	return out * sign;
}
