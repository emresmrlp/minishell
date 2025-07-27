/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:51:53 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/27 14:22:01 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(t_command *command, char **args)
{
	char	*path;

	if (args[1])
		path = args[1];
	else
	{
		path = find_env_value(command, "HOME");
		if (!path)
			return (error_handler("cd: HOME Not set\n"));
	}
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	return (SUCCESS);
}
