#include "shell.h"

/**
 * my_exit - a function to exits the shell
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if inf.argv[0] != "exit"
 */
int my_exit(info_t *inf)
{
	int ch_exit;

	if (inf->argv[1])
	{
		ch_exit = erratoi(inf->argv[1]);
		if (ch_exit == -1)
		{
			inf->status = 2;
			error_perinter(inf, "Illegal number: ");
			eputs(inf->argv[1]);
			eputchar('\n');
			return (1);
		}
		inf->err_num = erratoi(inf->argv[1]);
		return (-2);
	}
	inf->err_num = -1;
	return (-2);
}

/**
 * my_dir - a function to changes the current directory of the process
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_dir(info_t *inf)
{
	char *ss, *dir, buffer[1024];
	int chdirret;

	ss = getcwd(buffer, 1024);
	if (!ss)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!inf->argv[1])
	{
		dir = getenvi(inf, "HOME=");
		if (!dir)
			chdirret = chdir((dir = getenvi(inf, "PWD=")) ? dir : "/");
		else
			chdirret = chdir(dir);
	}
	else if (_strcmp(inf->argv[1], "-") == 0)
	{
		if (!getenvi(inf, "OLDPWD="))
		{
			_puts(ss);
			_putchar('\n');
			return (1);
		}
		_puts(getenvi(inf, "OLDPWD=")), _putchar('\n');
		chdirret = chdir((dir = getenvi(inf, "OLDPWD=")) ? dir : "/");
	}
	else
	{
		chdirret = chdir(inf->argv[1]);
	}
	if (chdirret == -1)
	{
		error_perinter(inf, "can't cd to ");
		eputs(inf->argv[1]), eputchar('\n');
	}
	else
	{
		set_envi(inf, "OLDPWD", getenvi(inf, "PWD="));
		set_envi(inf, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _help - a function that changes the current directory of the process
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _help(info_t *inf)
{
	char **arg_array;

	arg_array = inf->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array);
	return (0);
}

