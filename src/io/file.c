#include "context.h"
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

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
	// set path
	out->path = path;
	out->name = name == NULL ? name : strdup(name);

	// run stat
	int stat_ret = 0;
	if (ctx->ops.follow_links)
		stat_ret = stat(out->path, &(out->stat));
	else
		stat_ret = lstat(out->path, &(out->stat));
	if (stat_ret == -1) {
		// TODO temp
		perror(strerror(errno));
		return false;
	}

	// set other properties
	out->is_dir = S_ISDIR(out->stat.st_mode);
	out->is_relative = false;
	out->is_dotfile = false;
	if (out->is_dir && name != NULL)
		out->is_relative = strcmp(name, "..") == 0 || strcmp(name, ".") == 0;
	out->has_sorting_priority = out->is_relative;
	if (name != NULL)
		out->is_dotfile = name[0] == '.';
	return true;
}
