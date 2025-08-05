/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	check_file_access(t_command *command)
{
	struct stat	file_stat;

	if (stat(command->args[0], &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			error_handler(command, "minishell: Is a directory\n", 126);
			return (0);
		}
	}
	return (1);
}

static char	*handle_absolute_path(t_command *command)
{
	char	*path;

	if (access(command->args[0], F_OK) == 0)
	{
		if (!check_file_access(command))
			return (NULL);
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

static char	*handle_current_dir_path(t_command *command)
{
	char	*current_dir_path;

	current_dir_path = ft_strjoin("./", command->args[0]);
	if (access(current_dir_path, F_OK) == 0
		&& access(current_dir_path, X_OK) == 0)
	{
		free(current_dir_path);
		return (ft_strjoin("./", command->args[0]));
	}
	free(current_dir_path);
	if (!ft_strchr(command->args[0], '/'))
		error_handler(command, "minishell: command not found\n", 127);
	return (NULL);
}

char	*get_valid_path(t_command *command)
{
	char	*path;

	path = find_path(command->args[0], command);
	if (!path && ft_strchr(command->args[0], '/'))
		path = handle_absolute_path(command);
	if (!path)
		return (handle_current_dir_path(command));
	return (path);
}
