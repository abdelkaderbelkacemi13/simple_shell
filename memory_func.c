#include "shell.h"

/**
 **memoset - fills memory with a constant byte
 *@s: the pointer to the memory area
 *@b: the byte to fill *s with
 *@n: the amount of bytes to be filled
 *Return: (s) a pointer to the memory area s
 */
char *memoset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return (s);
}

/**
 * fr_free - frees a string of strings
 * @pp: string of strings
 */
void fr_free(char **pp)
{
	char **a = pp;

	if (!pp)
		return;
	while (*pp)
		free(*pp++);
	free(a);
}

/**
 * _real_aloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 *
 * Return: pointer to da ol'block nameen.
 */
void *_real_aloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *po;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	po = malloc(new_size);
	if (!po)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		po[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (po);
}

