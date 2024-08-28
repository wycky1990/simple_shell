#include "shell.h"

#define READ_BUF_SIZE 1024

/**
 * get_file_size - Gets the size of a file.
 * @fd: File descriptor of the file.
 *
 * Return: Size of the file.
 */
static ssize_t get_file_size(int fd)
{
	struct stat st;

	if (fstat(fd, &st) == -1)
		return (-1);

	return (st.st_size);
}

/**
 * read_file - Reads the content of a file into a buffer.
 * @fd: File descriptor of the file.
 * @size: Size of the file.
 *
 * Return: Pointer to the buffer containing file content.
 */
static char *read_file(int fd, ssize_t size)
{
	char *buf = malloc(size + 1);
	ssize_t rdlen;

	if (!buf)
		return (NULL);

	rdlen = read(fd, buf, size);
	buf[size] = '\0';

	if (rdlen <= 0)
	{
		free(buf);
		return (NULL);
	}

	return (buf);
}

/**
 * parse_history - Parses the history buffer and adds entries to the list.
 * @info: Structure containing potential arguments. Used to maintain
 *        the history linked list.
 * @buf: Buffer containing the history entries.
 * @size: Size of the buffer.
 *
 * Return: Number of lines added to the history list.
 */
static int parse_history(info_t *info, char *buf, ssize_t size)
{
	int i, last = 0, linecount = 0;

	for (i = 0; i < size; i++)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	}

	if (last != i)
		build_history_list(info, buf + last, linecount++);

	return (linecount);
}

/**
 * read_history - Reads history from a file.
 * @info: The parameter struct.
 *
 * Return: Number of lines read on success, 0 otherwise.
 */
int read_history(info_t *info)
{
	int fd;
	ssize_t size;
	char *buf = NULL;
	char *filename = get_history_file(info);
	int linecount = 0;

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);

	if (fd == -1)
		return (0);

	size = get_file_size(fd);
	if (size < 2)
	{
		close(fd);
		return (0);
	}

	buf = read_file(fd, size);
	close(fd);

	if (!buf)
		return (0);

	linecount = parse_history(info, buf, size);
	free(buf);

	info->histcount = linecount;

	while (info->histcount >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);
	return (info->histcount);
}
