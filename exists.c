#include "shell.h"

/**
 **_strncpy - function that copies a string
 *@dest: the destination string to be copied to
 *@src: the source string
 *@n: the amount of characters to be copied
 *Return: the concatenated string
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i, a;
	char *s = dest;

	i = 0;
	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < n)
	{
		a = i;
		while (a < n)
		{
			dest[a] = '\0';
			a++;
		}
	}
	return (s);
}

/**
 **_strncat - function that concatenates two strings
 *@dest: the first string
 *@src: the second string
 *@n: the amount of bytes to be maximally used
 *Return: the concatenated string
 */

char *_strncat(char *dest, char *src, int n)
{
	int i, a;
	char *s = dest;

	i = 0;
	a = 0;
	while (dest[i] != '\0')
		i++;
	while (src[a] != '\0' && a < n)
	{
		dest[i] = src[a];
		i++;
		a++;
	}
	if (a < n)
		dest[i] = '\0';
	return (s);
}

/**
 **_strchr - function that locates a character in a string
 *@s: the string to be parsed
 *@c: the character to look for
 *Return: (s) a pointer to the memory area s
 */

char *_strchr(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
