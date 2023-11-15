#include "shell.h"

/**
 * eputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		eputchar(str[i]);
		i++;
	}
}

/**
 * eputchar - writes the character ch to stderr
 * @ch: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int eputchar(char ch)
{
	static int a;
	static char buff[WRITE_BUF_SIZE];

	if (ch == BUF_FLUSH || a >= WRITE_BUF_SIZE)
	{
		write(2, buff, a);
		a = 0;
	}
	if (ch != BUF_FLUSH)
		buff[a++] = ch;
	return (1);
}

/**
 * putfd - writes the character ch to given ffd
 * @ch: The character to print
 * @ffd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int putfd(char ch, int ffd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (ch == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(ffd, buf, i);
		i = 0;
	}
	if (ch != BUF_FLUSH)
		buf[i++] = ch;
	return (1);
}

/**
 * puts_fd - a function used to prints an input string
 * @str: the string to be printed
 * @ffd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int puts_fd(char *str, int ffd)
{
	int a = 0;

	if (!str)
		return (0);
	while (*str)
	{
		a += putfd(*str++, ffd);
	}
	return (a);
}

