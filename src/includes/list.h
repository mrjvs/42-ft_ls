#ifndef FTLS_LIST
#define FTLS_LIST

#include <stddef.h>
#include <stdlib.h>

#define container_of(ptr, type, member) ({                      \
                const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
                (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct			s_dl_list {
	struct s_dl_list	*prev;
	struct s_dl_list	*next;
}						dl_list;

typedef struct			s_l_list {
	struct s_l_list	*next;
}						l_list;

/**
 * get data from list head. (member must be named "head")
 * returns type passed in
 */
#define	get_list_data(list, type) container_of(list, type, head)

/**
 * get next list item
 * returns list head
 */
#define	get_next_list(list) list->next

/**
 * get head of list entry
 * returns list head
 */
#define	get_list_head(entry) entry ? &(entry->head) : 0

l_list	*l_list_first(l_list *lst);
dl_list	*dl_list_first(dl_list *lst);

l_list	*l_list_last(l_list *lst);
dl_list	*dl_list_last(dl_list *lst);

void	l_list_push_back(l_list *lst, l_list *node);
void	dl_list_push_back(dl_list *lst, dl_list *node);

void	l_list_push_front(l_list *lst, l_list *node);
void	dl_list_push_front(dl_list *lst, dl_list *node);

void	l_list_push_after(l_list *node, l_list *new_node);
void	dl_list_push_after(dl_list *node, dl_list *new_node);

void	l_list_init_head(l_list *head);
void	dl_list_init_head(dl_list *head);

size_t	l_list_size(l_list *head);
size_t	dl_list_size(dl_list *head);

#endif
