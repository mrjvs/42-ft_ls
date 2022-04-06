#include "context.h"
#include "io.h"

/**
 * retrieve file information from a path
 * it may follow links if its set in the context
 * 
 * file information is outputted to the struct pointed to in 'out'
 * 
 * it returns truthy if successful
 * errno is set on error
 */
t_bool retrieve_file_info(ftls_context *ctx, char *path, ftls_file_info *out)
{
	// set path
	out->path = path;

	// run stat
	int stat_ret = 0;
	if (ctx->ops.follow_links)
		stat_ret = stat(out->path, &(out->stat));
	else
		stat_ret = lstat(out->path, &(out->stat));
	if (stat_ret == -1)
		return false;
	
	// set other properties
	out->is_dir = S_ISDIR(out->stat.st_mode);
	return true;
}
