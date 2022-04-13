#include <stdio.h>
#include <dirent.h>
#include "string.h"
#include "bool.h"
#include "io.h"
#include "path.h"

/**
 * gather files inside directory and stat them
 */
t_bool	gather_directory(ftls_context *ctx, char *path, ftls_dir *out)
{
	l_list_init_head(&(out->files));
	out->name = ftls_strdup(path); // TODO strdup

	DIR *folder = opendir(path);
    if (folder == NULL)
    {
		// TODO handle errors
        return false;
    }

    struct dirent *entry;
	while ((entry = readdir(folder)))
	{
		struct s_ftls_dir_entry *list_entry = malloc(sizeof(struct s_ftls_dir_entry));
		char *entry_path = path_join(path, entry->d_name);
		if (!list_entry || !retrieve_file_info(ctx, entry_path, entry->d_name, &(list_entry->file)))
		{
			if (list_entry) free(list_entry);
			// TODO free
			return false;
		}
		l_list_push_front(&(out->files), get_list_head(list_entry));
    }

    closedir(folder);
    return true;
}

/**
 * compose directory from input files and stat them
 */
t_bool	gather_composed_directory(ftls_context *ctx, int argc, char **argv, ftls_dir *out)
{
	l_list_init_head(&(out->files));
	out->name = NULL;

	for (int i = 0; i < argc; i++)
	{
		struct s_ftls_dir_entry *list_entry = malloc(sizeof(struct s_ftls_dir_entry));
		if (!list_entry || !retrieve_file_info(ctx, argv[i], argv[i], &(list_entry->file)))
		{
			if (list_entry) free(list_entry);
			// TODO free
			return false;
		}
		l_list_push_front(&(out->files), get_list_head(list_entry));
	}

    return true;
}
