/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/29 16:03:37 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_builtin(t_command *command)
{
	if (ft_strcmp(command->args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(command->args[0], "echo") == 0)
		return (builtin_echo(command));
	else if (ft_strcmp(command->args[0], "cd") == 0)
		return (builtin_cd(command));
	else if (ft_strcmp(command->args[0], "export") == 0)
		return (builtin_export(command, command->args));
	else if (ft_strcmp(command->args[0], "unset") == 0)
	{
		builtin_unset(command, command->args);
		return (SUCCESS);  // unset always returns 0 in bash
	}
	else if (ft_strcmp(command->args[0], "env") == 0)
		return (builtin_env(command));
	else if (ft_strcmp(command->args[0], "exit") == 0)
		return (builtin_exit(command, command->args));
	return (FAILURE);
}
