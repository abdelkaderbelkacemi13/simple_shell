#include "shell.h"

/**
 * history_file - a function used to get the history file
 * @inf: parameter struct
 *
 * Return: allocated string containg history file
 */

char *history_file(info_t *inf)
{
	char *buff, *dirr;

	dirr = getenvi(inf, "HOME=");
	if (!dirr)
		return (NULL);
	buff = malloc(sizeof(char) * (_strlen(dirr) + _strlen(HIST_FILE) + 2));
	if (!buff)
		return (NULL);
	buff[0] = 0;
	_strcpy(buff, dirr);
	_strcat(buff, "/");
	_strcat(buff, HIST_FILE);
	return (buff);
}

/**
 * writehistory - a function used to creates a file.
 * @inf: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int writehistory(info_t *inf)
{
	ssize_t fd;
	char *filename = history_file(inf);
	list_t *nod = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (nod = inf->history; nod; nod = nod->next)
	{
		puts_fd(nod->str, fd);
		putfd('\n', fd);
	}
	putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * readhistory - a function used to read history from file
 * @inf: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int readhistory(info_t *inf)
{
	int i, las = 0, lcount = 0;
	ssize_t fd, rdleng, fsize = 0;
	struct stat st;
	char *buff = NULL, *filename = history_file(inf);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buff = malloc(sizeof(char) * (fsize + 1));
	if (!buff)
		return (0);
	rdleng = read(fd, buff, fsize);
	buff[fsize] = 0;
	if (rdleng <= 0)
		return (free(buff), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buff[i] == '\n')
		{
			buff[i] = 0;
			build_history_list(inf, buff + las, lcount++);
			las = i + 1;
		}
	if (las != i)
		build_history_list(inf, buff + las, lcount++);
	free(buff);
	inf->histcount = lcount;
	while (inf->histcount-- >= HIST_MAX)
		delete_node_index(&(inf->history), 0);
	renumber_history(inf);
	return (inf->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @inf: Structure containing potential arguments. Used to maintain
 * @buff: buffer
 * @lcount: the history lcount, histcount
 *
 * Return: Always 0
 */
int build_history_list(info_t *inf, char *buff, int lcount)
{
	list_t *nod = NULL;

	if (inf->history)
		nod = inf->history;
	node_end(&nod, buff, lcount);

	if (!inf->history)
		inf->history = nod;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @inf: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(info_t *inf)
{
	list_t *nod = inf->history;
	int i = 0;

	while (nod)
	{
		nod->num = i++;
		nod = nod->next;
	}
	return (inf->histcount = i);
}

