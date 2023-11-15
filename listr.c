#include "shell.h"

/**
 * addnode - adds a node to the start of the list
 * @hed: address of pointer to hed node
 * @str: str field of node
 * @nu: node index used by history
 *
 * Return: size of list
 */
list_t *addnode(list_t **hed, const char *str, int nu)
{
	list_t *new_hed;

	if (!hed)
		return (NULL);
	new_hed = malloc(sizeof(list_t));
	if (!new_hed)
		return (NULL);
	memoset((void *) new_hed, 0, sizeof(list_t));
	new_hed->num = nu;
	if (str)
	{
		new_hed->str = _strdup(str);
		if (!new_hed->str)
		{
			free(new_hed);
			return (NULL);
		}
	}
	new_hed->next = *hed;
	*hed = new_hed;
	return (new_hed);
}

/**
 * node_end - adds a node to the end of the list
 * @hed: address of pointer to hed node
 * @str: str field of node
 * @nu: node index used by history
 *
 * Return: size of list
 */
list_t *node_end(list_t **hed, const char *str, int nu)
{
	list_t *new_nod, *nod;

	if (!hed)
		return (NULL);

	nod = *hed;
	new_nod = malloc(sizeof(list_t));
	if (!new_nod)
		return (NULL);
	memoset((void *) new_nod, 0, sizeof(list_t));
	new_nod->num = nu;
	if (str)
	{
		new_nod->str = _strdup(str);
		if (!new_nod->str)
		{
			free(new_nod);
			return (NULL);
		}
	}
	if (nod)
	{
		while (nod->next)
			nod = nod->next;
		nod->next = new_nod;
	}
	else
		*hed = new_nod;
	return (new_nod);
}

/**
 * str_list - prints only the str element of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t str_list(const list_t *h)
{
	size_t s = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		s++;
	}
	return (s);
}

/**
 * delete_node_index - deletes node at given index
 * @hed: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_index(list_t **hed, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!hed || !*hed)
		return (0);

	if (!index)
	{
		node = *hed;
		*hed = (*hed)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *hed;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * freelist - frees all nodes of a list
 * @hed_ptr: address of pointer to head node
 *
 * Return: void
 */
void freelist(list_t **hed_ptr)
{
	list_t *node, *next_node, *head;

	if (!hed_ptr || !*hed_ptr)
		return;
	head = *hed_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*hed_ptr = NULL;
}
