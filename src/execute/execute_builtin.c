/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 20:51:32 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_builtin(t_command *command)
{
	int result;
	
	if (ft_strcmp(command->args[0], "pwd") == 0)
		result = builtin_pwd();
	else if (ft_strcmp(command->args[0], "echo") == 0)
		result = builtin_echo(command);
	else if (ft_strcmp(command->args[0], "cd") == 0)
		result = builtin_cd(command);
	else if (ft_strcmp(command->args[0], "export") == 0)
		result = builtin_export(command, command->args);
	else if (ft_strcmp(command->args[0], "unset") == 0)
	{
		builtin_unset(command, command->args);
		result = SUCCESS;  // unset always returns 0 in bash
	}
	else if (ft_strcmp(command->args[0], "env") == 0)
		result = builtin_env(command);
	else if (ft_strcmp(command->args[0], "exit") == 0)
		result = builtin_exit(command, command->args);
	else
		result = FAILURE;
		
	// Tüm builtin komutlardan sonra static memory cleanup
	cleanup_exit_status_str(command);
	
	// Value'su olmayan environment variable'ları temizle
	cleanup_empty_env_vars(&command->env_list);
	
	return (result);
}
