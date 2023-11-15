#include "shell.h"

/**
 * hsh - main shell loop
 * @inf: the parameter & return inf struct
 * @avr: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *inf, char **avr)
{
	ssize_t s = 0;
	int builtin_ret = 0;

	while (s != -1 && builtin_ret != -2)
	{
		clearinfo(inf);
		if (interactive(inf))
			_puts("$ ");
		eputchar(BUF_FLUSH);
		s = getinput(inf);
		if (s != -1)
		{
			setinfo(inf, avr);
			builtin_ret = findbuiltin(inf);
			if (builtin_ret == -1)
				findcmd(inf);
		}
		else if (interactive(inf))
			_putchar('\n');
		freeinfo(inf, 0);
	}
	writehistory(inf);
	freeinfo(inf, 1);
	if (!interactive(inf) && inf->status)
		exit(inf->status);
	if (builtin_ret == -2)
	{
		if (inf->err_num == -1)
			exit(inf->status);
		exit(inf->err_num);
	}
	return (builtin_ret);
}

/**
 * findbuiltin - finds a builtin command
 * @inf: the parameter & return inf struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findbuiltin(info_t *inf)
{
	int i, builtinret = -1;
	builtin_table builtintbl[] = {
		{"exit",     my_exit},
		{"env",      myenvi},
		{"help",     _help},
		{"history",  _history},
		{"setenv",   stenvi},
		{"unsetenv", unsetenvi},
		{"cd",       my_dir},
		{"alias",    my_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(inf->argv[0], builtintbl[i].type) == 0)
		{
			inf->line_count++;
			builtinret = builtintbl[i].func(inf);
			break;
		}
	return (builtinret);
}

/**
 * findcmd - finds a command in PATH
 * @inf: the parameter & return inf struct
 *
 * Return: void
 */
void findcmd(info_t *inf)
{
	char *path = NULL;
	int i, k;

	inf->path = inf->argv[0];
	if (inf->linecount_flag == 1)
	{
		inf->line_count++;
		inf->linecount_flag = 0;
	}
	for (i = 0, k = 0; inf->arg[i]; i++)
		if (!isdelim(inf->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = path_finder(inf, getenvi(inf, "PATH="), inf->argv[0]);
	if (path)
	{
		inf->path = path;
		cmdfork(inf);
	}
	else
	{
		if ((interactive(inf) || getenvi(inf, "PATH=")
					|| inf->argv[0][0] == '/') && iscmd(inf, inf->argv[0]))
			cmdfork(inf);
		else if (*(inf->arg) != '\n')
		{
			inf->status = 127;
			error_perinter(inf, "not found\n");
		}
	}
}

/**
 * cmdfork - forks a an exec thread to run cmd
 * @inf: the parameter & return inf struct
 *
 * Return: void
 */
void cmdfork(info_t *inf)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{

		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(inf->path, inf->argv, get_envi(inf)) == -1)
		{
			freeinfo(inf, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}

	}
	else
	{
		wait(&(inf->status));
		if (WIFEXITED(inf->status))
		{
			inf->status = WEXITSTATUS(inf->status);
			if (inf->status == 126)
				error_perinter(inf, "Permission denied\n");
		}
	}
}
