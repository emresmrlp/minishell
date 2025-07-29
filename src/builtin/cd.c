/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:51:53 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 15:58:22 by makpolat         ###   ########.fr       */
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
			error_handler("cd: HOME not set\n");
			free(current_pwd);
			return (NULL);
		}
	}
	else if (ft_strcmp(iter->args[1], "-") == 0)
	{
		target_dir = find_env_value(iter, "OLDPWD");
		if (!target_dir)
		{
			error_handler("cd: OLDPWD not set\n");
			free(current_pwd);
			return (NULL);
		}
	}
	else
		target_dir = iter->args[1];
	return (target_dir);
}

int	builtin_cd(t_command *iter)
{
	char	*current_pwd;
	char	*target_dir;
	char	*new_pwd;
	int		arg_count;

	// Argüman sayısını kontrol et
	arg_count = 0;
	while (iter->args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		error_handler("cd: too many arguments\n");
		return (FAILURE);
	}

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
