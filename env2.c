#include "shell.h"

/**
 * get_envi - returns the string array copy of our environ
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_envi(info_t *inf)
{
	if (!inf->environ || inf->env_changed)
	{
		inf->environ = strings_list(inf->env);
		inf->env_changed = 0;
	}

	return (inf->environ);
}

/**
 * unset_envi - Remove an environment variable
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @va: the string env va property
 */
int unset_envi(info_t *inf, char *va)
{
	list_t *nod = inf->env;
	size_t i = 0;
	char *po;

	if (!nod || !va)
		return (0);

	while (nod)
	{
		po = starts_with(nod->str, va);
		if (po && *po == '=')
		{
			inf->env_changed = delete_node_index(&(inf->env), i);
			i = 0;
			nod = inf->env;
			continue;
		}
		nod = nod->next;
		i++;
	}
	return (inf->env_changed);
}

/**
 * set_envi - Initialize a new environment variable,
 *             or modify an existing one
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @va: the string env va property
 * @val: the string env va val
 *  Return: Always 0
 */
int set_envi(info_t *inf, char *va, char *val)
{
	char *buff = NULL;
	list_t *nod;
	char *po;

	if (!va || !val)
		return (0);

	buff = malloc(_strlen(va) + _strlen(val) + 2);
	if (!buff)
		return (1);
	_strcpy(buff, va);
	_strcat(buff, "=");
	_strcat(buff, val);
	nod = inf->env;
	while (nod)
	{
		po = starts_with(nod->str, va);
		if (po && *po == '=')
		{
			free(nod->str);
			nod->str = buff;
			inf->env_changed = 1;
			return (0);
		}
		nod = nod->next;
	}
	node_end(&(inf->env), buff, 0);
	free(buff);
	inf->env_changed = 1;
	return (0);
}

