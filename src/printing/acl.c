#include "io.h"
#include <sys/stat.h>

static char	get_acl_dir(ftls_file_info *file) {
	if (S_ISLNK(file->stat.st_mode))
		return 'l';
	if (S_ISDIR(file->stat.st_mode))
		return 'd';
	return '-';
}

// TODO extended attributes + special chars https://www.unix.com/unix-for-dummies-questions-and-answers/41832-ls-l-column-headings.html
char	*get_acl(ftls_file_info *file) {
	char *out = malloc(12);
	if (!out)
		return NULL;
	int i = 0;
	out[i++] = get_acl_dir(file);

	// user RWX
	out[i++] = file->stat.st_mode & S_IRUSR ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWUSR ? 'w' : '-';
	out[i++] = file->stat.st_mode & S_IXUSR ? 'x' : '-';

	// group RWX
	out[i++] = file->stat.st_mode & S_IRGRP ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWGRP ? 'w' : '-';
	out[i++] = file->stat.st_mode & S_IXGRP ? 'x' : '-';

	// other RWX
	out[i++] = file->stat.st_mode & S_IROTH ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWOTH ? 'w' : '-';
	out[i++] = file->stat.st_mode & S_IXOTH ? 'x' : '-';

	out[i++] = 0;
	return out;
}
