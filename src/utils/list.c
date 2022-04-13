#include "list.h"

/**
 * Free a linked list
 * the del argument is a function that receives a list head to free the container from
 */
void	free_l_list(l_list *lst, void (*del)(l_list *head))
{
	lst = lst->next;
	l_list *next;
	while (lst != NULL)
	{
		next = lst->next;
		del(lst);
		lst = next;
	}
}

/**
 * Free a double linked list
 * the del argument is a function that receives a list head to free the container from
 */
void	free_dl_list(dl_list *lst, void (*del)(dl_list *head))
{
	lst = lst->next;
	dl_list *next;
	while (lst != NULL)
	{
		next = lst->next;
		del(lst);
		lst = next;
	}
}

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
	node->next = lst;
}

/**
 * push a new node to the fron to of a double linked list
 */
void	dl_list_push_front(dl_list *lst, dl_list *node)
{
	node->next = lst;
	node->prev = NULL;
	lst->prev = node;
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
