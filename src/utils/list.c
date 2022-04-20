#include "list.h"
#include <stdio.h>

/**
 * Get first node from linked list
 */
l_list	*l_list_first(l_list *lst)
{
	return lst->next;
}

/**
 * get first node from double linked list
 */
dl_list	*dl_list_first(dl_list *lst)
{
	return lst;
}

/**
 * get last node from linked list
 */
l_list	*l_list_last(l_list *lst)
{
	l_list *prev;
	while ((lst = get_next_list(lst)))
		prev = lst;
	return prev;
}

/**
 * get last node from double linked list
 */
dl_list	*dl_list_last(dl_list *lst)
{
	dl_list *prev;
	while ((lst = get_next_list(lst)))
		prev = lst;
	return prev;	
}

/**
 * push a new node to the back of a linked list
 */
void	l_list_push_back(l_list *lst, l_list *node)
{
	l_list_push_after(l_list_last(lst), node);
}

/**
 * push a new node to the back of a double linked list
 */
void	dl_list_push_back(dl_list *lst, dl_list *node)
{
	dl_list_push_after(dl_list_last(lst), node);
}

/**
 * push a new node to the front of a linked list
 */
void	l_list_push_front(l_list *lst, l_list *node)
{
	node->next = lst->next;
	lst->next = node;
}

/**
 * push a new node to the fron to of a double linked list
 */
void	dl_list_push_front(dl_list *lst, dl_list *node)
{
	node->next = lst->next;
	node->next->prev = node;
	node->prev = NULL;
	lst->next = node;
}

/**
 * push a new node right after the pointed to node in a linked list
 */
void	l_list_push_after(l_list *node, l_list *new_node)
{
	new_node->next = node->next;
	node->next = new_node;
}

/**
 * push a new node right after the pointed to node in a double linked list
 */
void	dl_list_push_after(dl_list *node, dl_list *new_node)
{
	new_node->next = node->next;
	new_node->prev = node;

	if (new_node->next) new_node->next->prev = new_node;
	node->next = new_node; 
}

/**
 * initialize the head of a linked list
*/
void	l_list_init_head(l_list *head)
{
	head->next = NULL;
}

/**
 * initialize the head of a double linked list
*/
void	dl_list_init_head(dl_list *head)
{
	head->next = NULL;
	head->prev = NULL;
}

/**
 * Get linked list size
*/
size_t	l_list_size(l_list *head) {
	size_t i = 0;
	while((head = get_next_list(head)))
		i++;
	return i;
}

/**
 * Get double linked list size
*/
size_t	dl_list_size(dl_list *head) {
	size_t i = 0;
	while((head = get_next_list(head)))
		i++;
	return i;
}
