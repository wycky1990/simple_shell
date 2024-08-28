#include "shell.h"
#include <unistd.h>

#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/**
 * _strcpy - function that copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *_strcpy(char *dest, char *src)
{
	int a = 0;

	if (dest == src || src == NULL)
		return (dest);

	while (src[a])
	{
		dest[a] = src[a];
		a++;
	}
	dest[a] = '\0';

	return (dest);
}

/**
 * _strdup - function that duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *_strdup(const char *str)
{
	int length = 0;
	char *strs;

	if (str == NULL)
		return (NULL);

	while (*str++)
		length++;

	strs = malloc(sizeof(char) * (length + 1));
	if (!strs)
		return (NULL);

	for (length++; length--;)
		strs[length] = *--str;

	return (strs);
}

/**
 * _puts - function that prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _puts(char *str)
{
	int a = 0;

	if (!str)
		return;

	while (str[a] != '\0')
	{
		_putchar(str[a]);
		a++;
	}
}

/**
 * _putchar - function that writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;

	return (1);
}
