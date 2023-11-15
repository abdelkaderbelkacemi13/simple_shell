#include "shell.h"

/**
 * list_leng - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t list_leng(const list_t *h)
{
	size_t s = 0;

	while (h)
	{
		h = h->next;
		s++;
	}
	return (s);
}

/**
 * strings_list - returns an array of strings of the list->str
 * @hed: pointer to first node
 *
 * Return: array of strings
 */
char **strings_list(list_t *hed)
{
	list_t *nod = hed;
	size_t s = list_leng(hed), j;
	char **strs;
	char *str;

	if (!hed || !s)
		return (NULL);
	strs = malloc(sizeof(char *) * (s + 1));
	if (!strs)
		return (NULL);
	for (s = 0; nod; nod = nod->next, s++)
	{
		str = malloc(_strlen(nod->str) + 1);
		if (!str)
		{
			for (j = 0; j < s; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, nod->str);
		strs[s] = str;
	}
	strs[s] = NULL;
	return (strs);
}


/**
 * print_list - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_list(const list_t *h)
{
	size_t s = 0;

	while (h)
	{
		_puts(convert_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		s++;
	}
	return (s);
}

/**
 * node_starts_with - returns nod whose string starts with prfix
 * @nod: pointer to list head
 * @prfix: string to match
 * @ch: the next character after prfix to match
 *
 * Return: match nod or null
 */
list_t *node_starts_with(list_t *nod, char *prfix, char ch)
{
	char *po = NULL;

	while (nod)
	{
		po = starts_with(nod->str, prfix);
		if (po && ((ch == -1) || (*po == ch)))
			return (nod);
		nod = nod->next;
	}
	return (NULL);
}

/**
 * node_index - gets the index of a nod
 * @hed: pointer to list hed
 * @nod: pointer to the nod
 *
 * Return: index of nod or -1
 */
ssize_t node_index(list_t *hed, list_t *nod)
{
	size_t i = 0;

	while (hed)
	{
		if (hed == nod)
			return (i);
		hed = hed->next;
		i++;
	}
	return (-1);
}

