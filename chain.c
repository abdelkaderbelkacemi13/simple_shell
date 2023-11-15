#include "shell.h"

/**
 * ischain - a function to test if the current 
 * char in buffer is a chain delimeter
 * @inf: the parameter struct
 * @buff: the char buffer
 * @po: address of current position in buff
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int ischain(info_t *inf, char *buff, size_t *po)
{
	size_t j = *po;

	if (buff[j] == '|' && buff[j + 1] == '|')
	{
		buff[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_OR;
	}
	else if (buff[j] == '&' && buff[j + 1] == '&')
	{
		buff[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_AND;
	}
	else if (buff[j] == ';')
	{
		buff[j] = 0;
		inf->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*po = j;
	return (1);
}

/**
 * chaincheck - a function to check we should 
 * continue chaining based on last status
 * @inf: the parameter struct
 * @buff: the char buffer
 * @po: address of current position in buff
 * @a: starting position in buff
 * @leng: length of buff
 *
 * Return: Void
 */
void chaincheck(info_t *inf, char *buff, size_t *po, size_t a, size_t leng)
{
	size_t j = *po;

	if (inf->cmd_buf_type == CMD_AND)
	{
		if (inf->status)
		{
			buff[a] = 0;
			j = leng;
		}
	}
	if (inf->cmd_buf_type == CMD_OR)
	{
		if (!inf->status)
		{
			buff[a] = 0;
			j = leng;
		}
	}
	*po = j;
}

/**
 * replacealias - a function use to replace an aliases in the tokenized string
 * @inf: the parameter struct
 * Return: 1 if replaced, 0 otherwise
 */
int replacealias(info_t *inf)
{
	int i;
	list_t *nod;
	char *po;

	for (i = 0; i < 10; i++)
	{
		nod = node_starts_with(inf->alias, inf->argv[0], '=');
		if (!nod)
			return (0);
		free(inf->argv[0]);
		po = _strchr(nod->str, '=');
		if (!po)
			return (0);
		po = _strdup(po + 1);
		if (!po)
			return (0);
		inf->argv[0] = po;
	}
	return (1);
}

/**
 * vars_replace -a function used to replace vars in the tokenized string
 * @inf: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int vars_replace(info_t *inf)
{
	int i = 0;
	list_t *nod;

	for (i = 0; inf->argv[i]; i++)
	{
		if (inf->argv[i][0] != '$' || !inf->argv[i][1])
			continue;
		if (!_strcmp(inf->argv[i], "$?"))
		{
			str_rplace(&(inf->argv[i]),
					_strdup(convert_number(inf->status, 10, 0)));
			continue;
		}
		if (!_strcmp(inf->argv[i], "$$"))
		{
			str_rplace(&(inf->argv[i]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		nod = node_starts_with(inf->env, &inf->argv[i][1], '=');
		if (nod)
		{
			str_rplace(&(inf->argv[i]),
					_strdup(_strchr(nod->str, '=') + 1));
			continue;
		}
		str_rplace(&inf->argv[i], _strdup(""));
	}
	return (0);
}

/**
 * str_replace - a function used to replace a string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int str_rplace(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

