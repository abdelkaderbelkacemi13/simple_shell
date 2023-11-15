#include "shell.h"

/**
 * clearinfo - initializes info_t struct
 * @inf: struct address
 */
void clearinfo(info_t *inf)
{
	inf->arg = NULL;
	inf->argv = NULL;
	inf->path = NULL;
	inf->argc = 0;
}

/**
 * setinfo - initializes info_t struct
 * @inf: struct address
 * @avr: argument vector
 */
void setinfo(info_t *inf, char **avr)
{
	int i = 0;

	inf->fname = avr[0];
	if (inf->arg)
	{
		inf->argv = strtow(inf->arg, " \t");
		if (!inf->argv)
		{
			inf->argv = malloc(sizeof(char *) * 2);
			if (inf->argv)
			{
				inf->argv[0] = _strdup(inf->arg);
				inf->argv[1] = NULL;
			}
		}
		for (i = 0; inf->argv && inf->argv[i]; i++)
			;
		inf->argc = i;

		replacealias(inf);
		vars_replace(inf);
	}
}

/**
 * freeinfo - frees info_t struct fields
 * @inf: struct address
 * @all: true if freeing all fields
 */
void freeinfo(info_t *inf, int all)
{
	fr_free(inf->argv);
	inf->argv = NULL;
	inf->path = NULL;

	if (all)
	{
		if (!inf->cmd_buf)
			free(inf->arg);
		if (inf->env)
			freelist(&(inf->env));
		if (inf->history)
			freelist(&(inf->history));
		if (inf->alias)
			freelist(&(inf->alias));
		fr_free(inf->environ);
		inf->environ = NULL;
		bfree((void **)inf->cmd_buf);
		if (inf->readfd > 2)
			close(inf->readfd);
		_putchar(BUF_FLUSH);
	}
}
