#include "io.h"
#include <sys/stat.h>

static char	get_perms_dir(ftls_file_info *file) {
	if (S_ISLNK(file->stat.st_mode))
		return 'l';
	if (S_ISDIR(file->stat.st_mode))
		return 'd';
	if (S_ISCHR(file->stat.st_mode))
		return 'c';
	if (S_ISFIFO(file->stat.st_mode))
		return 'p';
	if (S_ISBLK(file->stat.st_mode))
		return 'b';
	if (S_ISSOCK(file->stat.st_mode))
		return 's';
	return '-';
}

static char	get_perms_exec_user(ftls_file_info *file) {
	if ((file->stat.st_mode & ( S_ISUID | S_IXUSR )) == (S_ISUID | S_IXUSR))
		return 's';
	if (file->stat.st_mode & S_ISUID)
		return 'S';
	if (file->stat.st_mode & S_IXUSR)
		return 'x';
	return '-';
}

static char	get_perms_exec_group(ftls_file_info *file) {
	if ((file->stat.st_mode & ( S_ISGID | S_IXGRP )) == (S_ISGID | S_IXGRP))
		return 's';
	if (file->stat.st_mode & S_ISGID)
		return 'S';
	if (file->stat.st_mode & S_IXGRP)
		return 'x';
	return '-';
}

static char	get_perms_exec_other(ftls_file_info *file) {
	if ((file->stat.st_mode & ( S_ISVTX | S_IXOTH )) == (S_ISVTX | S_IXOTH))
		return 't';
	if (file->stat.st_mode & S_ISVTX)
		return 'T';
	if (file->stat.st_mode & S_IXOTH)
		return 'x';
	return '-';
}

char	*get_perms(ftls_file_info *file) {
	char *out = malloc(12);
	if (!out)
		return NULL;
	int i = 0;
	out[i++] = get_perms_dir(file);

	// user RWX
	out[i++] = file->stat.st_mode & S_IRUSR ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWUSR ? 'w' : '-';
	out[i++] = get_perms_exec_user(file);

	// group RWX
	out[i++] = file->stat.st_mode & S_IRGRP ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWGRP ? 'w' : '-';
	out[i++] = get_perms_exec_group(file);

	// other RWX
	out[i++] = file->stat.st_mode & S_IROTH ? 'r' : '-';
	out[i++] = file->stat.st_mode & S_IWOTH ? 'w' : '-';
	out[i++] = get_perms_exec_other(file);

	// extended attributes
	if (file->has_xattr)
		out[i++] = '@';
	else if (file->has_acl)
		out[i++] = '+';
#ifdef MACOS
	else
		out[i++] = ' ';
#endif

	out[i++] = 0;
	return out;
}
