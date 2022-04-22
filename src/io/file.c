#include "context.h"
#include "io.h"
#include "ftls_string.h"
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

/**
 * retrieve file information from a path
 * it may follow links if its set in the context
 * 
 * file information is outputted to the struct pointed to in 'out'
 * 
 * it returns truthy if successful
 * errno is set on error
 */
t_bool retrieve_file_info(ftls_context *ctx, char *path, char *name, ftls_file_info *out)
{
	// set path & name
	out->path = ftls_strdup(path);
	if (!out->path)
		return false;
	out->name = NULL;
	if (name) {
		out->name = ftls_strdup(name);
		if (!out->name) {
			free(out->path);
			return false;
		}
	}

	// run stat
	int stat_ret = 0;
	if (ctx->ops.follow_links)
		stat_ret = stat(out->path, &(out->stat));
	else
		stat_ret = lstat(out->path, &(out->stat));
	if (stat_ret == -1) {
		free(out->name);
		free(out->path);
		return false;
	}


	// set other properties
	out->is_exec = out->stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH);
	out->is_dir = S_ISDIR(out->stat.st_mode);
	out->is_relative = false;
	out->is_dotfile = false;
	if (out->is_dir && name != NULL)
		out->is_relative = ftls_strcmp(name, "..") == 0 || ftls_strcmp(name, ".") == 0;
	out->has_sorting_priority = out->is_relative;
	if (name != NULL)
		out->is_dotfile = name[0] == '.';

	// colors
	out->color = C_FILE;
	if (out->is_exec) out->color = C_EXEC;
	if (out->is_exec && out->stat.st_mode & S_ISUID) out->color = C_EXEC_SETUID;
	if (out->is_exec && out->stat.st_mode & S_ISGID) out->color = C_EXEC_SETGID;
	if (S_ISLNK(out->stat.st_mode)) out->color = C_SYMLINK;
	if (S_ISSOCK(out->stat.st_mode)) out->color = C_SOCKET;
	if (S_ISFIFO(out->stat.st_mode)) out->color = C_PIPE;
	if (S_ISBLK(out->stat.st_mode)) out->color = C_BLOCK_S;
	if (S_ISCHR(out->stat.st_mode)) out->color = C_CHAR_S;
	if (S_ISDIR(out->stat.st_mode)) out->color = C_DIR;
	if (S_ISDIR(out->stat.st_mode) && out->stat.st_mode & S_IWOTH) out->color = C_DIR_WRITABLE;
	if (S_ISDIR(out->stat.st_mode) && out->stat.st_mode & (S_IWOTH | S_ISVTX)) out->color = C_DIR_WRITABLE_STICKY;
	return true;
}

/**
 * Free a ftls_file_info struct
*/
void	free_file_info(ftls_file_info *file) {
	if (!file)
		return;
	free(file->path);
	free(file->name);
}
