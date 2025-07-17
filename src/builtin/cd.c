/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:51:53 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 00:36:09 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(t_command *iter)
{
	char	*current_pwd = getcwd(NULL, 0);
	char	*target_dir;
	char	*new_pwd;

	if (!iter->args[1] || ft_strcmp(iter->args[1], "~") == 0)
		target_dir = find_env_value(iter, "HOME");
	else if (ft_strcmp(iter->args[1], "-") == 0)
	{
		target_dir = find_env_value(iter, "OLDPWD");
		if (!target_dir)
		{
			printf("cd: OLDPWD not set\n");
			free(current_pwd);
			return (FAILURE);
		}
	}
	else
		target_dir = iter->args[1];
	if (chdir(target_dir) == -1)
	{
		perror("cd");
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
