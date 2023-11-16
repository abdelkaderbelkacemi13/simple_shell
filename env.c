#include "shell.h"

/**
 * myenvi - a function to print the current environment
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int myenvi(info_t *inf)
{
	str_list(inf->env);
	return (0);
}

/**
 * getenvi - a function used to  get the value of an environ variable
 * @inf: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *getenvi(info_t *inf, const char *name)
{
	list_t *nod = inf->env;
	char *po;

	while (nod)
	{
		po = starts_with(nod->str, name);
		if (po && *po)
			return (po);
		nod = nod->next;
	}
	return (NULL);
}
/**
 * stenvi - a functio to Initialize a new environment variable,
 *             or modify an existing one
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int stenvi(info_t *inf)
{
	if (inf->argc != 3)
	{
		eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (set_envi(inf, inf->argv[1], inf->argv[2]))
		return (0);
	return (1);
}

/**
 * unsetenvi - Remove an environment variable
 * @inf: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int unsetenvi(info_t *inf)
{
	int i;

	if (inf->argc == 1)
	{
		eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= inf->argc; i++)
		unset_envi(inf, inf->argv[i]);

	return (0);
}

/**
 * env_list -  env linked list
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int env_list(info_t *inf)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		node_end(&node, environ[i], 0);
	inf->env = node;
	return (0);
}

