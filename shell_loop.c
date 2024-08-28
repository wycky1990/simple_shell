#include "shell.h"

/**
 * hsh - Main shell loop.
 * @info: The parameter and return info struct.
 * @av: The argument vector from main().
 *
 * Return: 0 on success, 1 on error, or error code.
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		prompt_user(info);
		r = get_input(info);

		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');

		free_info(info, 0);
	}

	return (handle_exit(info, builtin_ret));
}

/**
 * prompt_user - Prints the prompt if in interactive mode.
 * @info: The parameter and return info struct.
 */
void prompt_user(info_t *info)
{
	if (interactive(info))
	{
		_puts("$ ");
		_eputchar(BUF_FLUSH);
	}
}

/**
 * handle_exit - Handles the exit conditions after the main loop.
 * @info: The parameter and return info struct.
 * @builtin_ret: The return value of the built-in command.
 *
 * Return: Exit status.
 */
int handle_exit(info_t *info, int builtin_ret)
{
	write_history(info);
	free_info(info, 1);

	if (!interactive(info) && info->status)
		exit(info->status);

	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}

	return (builtin_ret);
}

/**
 * find_builtin - Finds a builtin command.
 * @info: The parameter and return info struct.
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit().
 */
int find_builtin(info_t *info)
{
	int i;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
	{
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			return (builtintbl[i].func(info));
		}
	}

	return (-1);
}

/**
 * find_cmd - Finds a command in PATH.
 * @info: The parameter and return info struct.
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int k = count_non_delim_args(info);

	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);

	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		handle_no_path(info);
	}
}

/**
 * count_non_delim_args - Counts the number of non-delimiter arguments.
 * @info: The parameter and return info struct.
 *
 * Return: Number of non-delimiter arguments.
 */
int count_non_delim_args(info_t *info)
{
	int i, k = 0;

	for (i = 0; info->arg[i]; i++)
	{
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	}

	return (k);
}

/**
 * handle_no_path - Handles cases where the command path is not found.
 * @info: The parameter and return info struct.
 */
void handle_no_path(info_t *info)
{
	if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
	{
		fork_cmd(info);
	}
	else if (*(info->arg) != '\n')
	{
		info->status = 127;
		print_error(info, "not found\n");
	}
}

/**
 * fork_cmd - Forks an exec thread to run the command.
 * @info: The parameter and return info struct.
 */
void fork_cmd(info_t *info)
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
		execute_command(info);
	}
	else
	{
		wait_for_child(info);
	}
}

/**
 * execute_command - Executes the command in the child process.
 * @info: The parameter and return info struct.
 */
void execute_command(info_t *info)
{
	if (execve(info->path, info->argv, get_environ(info)) == -1)
	{
		free_info(info, 1);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}

/**
 * wait_for_child - Waits for the child process to finish.
 * @info: The parameter and return info struct.
 */
void wait_for_child(info_t *info)
{
	wait(&(info->status));
	if (WIFEXITED(info->status))
	{
		info->status = WEXITSTATUS(info->status);
		if (info->status == 126)
			print_error(info, "Permission denied\n");
	}
}
