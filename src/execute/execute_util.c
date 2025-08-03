/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:36:28 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 15:52:20 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

char	*find_path(char *arg, t_command *command)
{
	char	*command_path;
	char	*temp;
	char	**path_array;
	char	**original_path_array;

	path_array = ft_split(find_env_value(command, "PATH"), ':');
	if (!path_array)
		return (NULL);
	original_path_array = path_array;
	while (*path_array)
	{
		temp = ft_strjoin(*path_array, "/");
		command_path = ft_strjoin(temp, arg);
		free(temp);
		if (access(command_path, X_OK) == 0)
		{
			free_array(original_path_array);
			return (command_path);
		}
		free(command_path);
		path_array++;
	}
	free_array(original_path_array);
	return (NULL);
}

static char	*handle_absolute_path(t_command *command)
{
	struct stat	file_stat;
	char		*path;

	if (access(command->args[0], F_OK) == 0)
	{
		if (stat(command->args[0], &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
			{
				error_handler(command, "minishell: Is a directory\n", 126);
				return (NULL);
			}
		}
		if (access(command->args[0], X_OK) == 0)
			path = ft_strdup(command->args[0]);
		else
		{
			error_handler(command, "minishell: Permission denied\n", 126);
			return (NULL);
		}
	}
	else
	{
		error_handler(command, "minishell: No such file or directory\n", 127);
		return (NULL);
	}
	return (path);
}

char	*get_valid_path(t_command *command)
{
	char	*path;

	path = find_path(command->args[0], command);
	if (!path && ft_strchr(command->args[0], '/'))
		path = handle_absolute_path(command);
	if (!path)
	{
		error_handler(command, "minishell: command not found\n", 127);
		return (NULL);
	}
	return (path);
}
