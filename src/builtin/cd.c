/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:51:53 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 20:58:43 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*target_directory(t_command *iter, char *current_pwd)
{
	char	*target_dir;

	if (!iter->args[1] || ft_strcmp(iter->args[1], "~") == 0)
	{
		target_dir = find_env_value(iter, "HOME");
		if (!target_dir)
		{
			error_handler(iter, "cd: HOME not set\n");
			free(current_pwd);
			return (NULL);
		}
	}
	else if (ft_strcmp(iter->args[1], "-") == 0)
	{
		target_dir = find_env_value(iter, "OLDPWD");
		if (!target_dir)
		{
			error_handler(iter, "cd: OLDPWD not set\n");
			free(current_pwd);
			return (NULL);
		}
	}
	else
		target_dir = iter->args[1];
	return (target_dir);
}

int	arg_check(t_command *command)
{
	int		arg_count;

	arg_count = 0;
	while (command->args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		error_handler(command, "cd: too many arguments\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	builtin_cd(t_command *iter)
{
	char	*current_pwd;
	char	*target_dir;
	char	*new_pwd;
	
	
	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("getcwd");
		return (FAILURE);
	}
	target_dir = target_directory(iter, current_pwd);
	if (!target_dir)
		return (FAILURE);
	if (chdir(target_dir) == -1)
	{
		write(2, "cd: ", 4);
		write(2, target_dir, ft_strlen(target_dir));
		write(2, ": No such file or directory\n", 28);
		free(current_pwd);
		return (FAILURE);
	}
	update_env(iter, "OLDPWD", current_pwd);
	new_pwd = getcwd(NULL, 0);
	update_env(iter, "PWD", new_pwd);
	free(current_pwd);
	free(new_pwd);
	return (SUCCESS);
}
