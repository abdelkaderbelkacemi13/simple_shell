#include "shell.h"

/**
 * _history - a function that displays the history list, one command by line.
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _history(info_t *inf)
{
	print_list(inf->history);
	return (0);
}

/**
 * _unsetalias - a function used to set alias to string
 * @inf: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int _unsetalias(info_t *inf, char *str)
{
	char *po, ch;
	int rt;

	po = _strchr(str, '=');
	if (!po)
		return (1);
	ch = *po;
	*po = 0;
	rt = delete_node_index(&(inf->alias),
			node_index(inf->alias, node_starts_with(inf->alias, str, -1)));
	*po = ch;
	return (rt);
}

/**
 * _setalias - a function used to set alias to string
 * @inf: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int _setalias(info_t *inf, char *str)
{
	char *po;

	po = _strchr(str, '=');
	if (!po)
		return (1);
	if (!*++po)
		return (_unsetalias(inf, str));
	_unsetalias(inf, str);
	return (node_end(&(inf->alias), str, 0) == NULL);
}

/**
 * _printalias - a function used to print an alias string
 * @nod: the alias nod
 * Return: Always 0 on success, 1 on error
 */
int _printalias(list_t *nod)
{
	char *po = NULL, *aa = NULL;

	if (nod)
	{
		po = _strchr(nod->str, '=');
		for (aa = nod->str; aa <= po; aa++)
			_putchar(*aa);
		_putchar('\'');
		_puts(po + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias -a function to mimics the alias builtin
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_alias(info_t *inf)
{
	int i = 0;
	char *po = NULL;
	list_t *nod = NULL;

	if (inf->argc == 1)
	{
		nod = inf->alias;
		while (nod)
		{
			_printalias(nod);
			nod = nod->next;
		}
		return (0);
	}
	for (i = 1; inf->argv[i]; i++)
	{
		po = _strchr(inf->argv[i], '=');
		if (po)
			_setalias(inf, inf->argv[i]);
		else
			_printalias(node_starts_with(inf->alias, inf->argv[i], '='));
	}
	return (0);
}

