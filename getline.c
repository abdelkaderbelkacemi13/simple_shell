#include "shell.h"

/**
 * input_buf - used in input the buffers chained commands
 * @inf: parameter struct
 * @buff: address of buffer
 * @leng: address of leng var
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *inf, char **buff, size_t *leng)
{
	ssize_t r = 0;
	size_t leng_p = 0;

	if (!*leng)
	{

		free(*buff);
		*buff = NULL;
		signal(SIGINT, sigintHand);
#if USE_GETLINE
		r = getline(buff, &leng_p, stdin);
#else
		r = _getline(inf, buff, &leng_p);
#endif
		if (r > 0)
		{
			if ((*buff)[r - 1] == '\n')
			{
				(*buff)[r - 1] = '\0';
				r--;
			}
			inf->linecount_flag = 1;
			comments_remover(*buff);
			build_history_list(inf, *buff, inf->histcount++);

			{
				*leng = r;
				inf->cmd_buf = buff;
			}
		}
	}
	return (r);
}

/**
 * getinput - gets a line minus the newline
 * @inf: parameter struct
 *
 * Return: bytes read
 */
ssize_t getinput(info_t *inf)
{
	static char *buff;
	static size_t i, j, leng;
	ssize_t r = 0;
	char **buff_p = &(inf->arg), *po;

	_putchar(BUF_FLUSH);
	r = input_buf(inf, &buff, &leng);
	if (r == -1)
		return (-1);
	if (leng)
	{
		j = i;
		po = buff + i;

		chaincheck(inf, buff, &j, i, leng);
		while (j < leng)
		{
			if (ischain(inf, buff, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= leng)
		{
			i = leng = 0;
			inf->cmd_buf_type = CMD_NORM;
		}

		*buff_p = po;
		return (_strlen(po));
	}

	*buff_p = buff;
	return (r);
}

/**
 * readbuf - reads a buffer
 * @inf: parameter struct
 * @buff: buffer
 * @s: size
 *
 * Return: r
 */
ssize_t readbuf(info_t *inf, char *buff, size_t *s)
{
	ssize_t r = 0;

	if (*s)
		return (0);
	r = read(inf->readfd, buff, READ_BUF_SIZE);
	if (r >= 0)
		*s = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @inf: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @leng: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *inf, char **ptr, size_t *leng)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *po = NULL, *new_po = NULL, *c;

	po = *ptr;
	if (po && leng)
		s = *leng;
	if (i == len)
		i = len = 0;

	r = readbuf(inf, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_po = _real_aloc(po, s, s ? s + k : k + 1);
	if (!new_po)
		return (po ? free(po), -1 : -1);

	if (s)
		_strncat(new_po, buf + i, k - i);
	else
		_strncpy(new_po, buf + i, k - i + 1);

	s += k - i;
	i = k;
	po = new_po;

	if (leng)
		*leng = s;
	*ptr = po;
	return (s);
}

/**
 * sigintHand - used to block ctrl-C
 * @signum: the signal number
 *
 * Return: void
 */
void sigintHand(__attribute__((unused))int signum)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
