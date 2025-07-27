/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:36:28 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/27 14:00:04 by ysumeral         ###   ########.fr       */
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
