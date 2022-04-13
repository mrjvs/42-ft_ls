#include <stdio.h>
#include <dirent.h>
#include "bool.h"
#include "io.h"

static void	free_ftls_dir_entry(l_list *lst)
{
	free(get_list_data(lst, struct s_ftls_dir_entry));
}

/**
 * gather files inside directory and stat them
 */
t_bool	gather_directory(ftls_context *ctx, char *path, ftls_dir *out)
{
	out->files = 0;

	DIR *folder = opendir(path);
    if (folder == NULL)
    {
		// TODO handle errors
        return false;
    }

    struct dirent *entry;
    ftls_file_info file;
	while ((entry = readdir(folder)))
	{
		struct s_ftls_dir_entry *list_entry = malloc(sizeof(struct s_ftls_dir_entry));
		if (!list_entry || !retrieve_file_info(ctx, entry->d_name, &file))
		{
			if (list_entry) free(list_entry);
			free_l_list(get_list_head(out->files), free_ftls_dir_entry);
			return false;
		}
		l_list_push_front(get_list_head(out->files), get_list_head(list_entry));
		out->files = list_entry;
    }

    closedir(folder);
    return true;
}
