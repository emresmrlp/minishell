/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:31 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/30 17:56:05 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_arguments(t_command *command)
{
	int	total_args;

	if (!command->args[0] || ft_strcmp(command->args[0], "echo") != 0)
	{
		total_args = 0;
		while (command->args[total_args])
			total_args++;
		remove_empty_strings(command, total_args);
		compact_args_array(command, total_args);
	}
}

void	execute_single(t_command *command)
{
	if (handle_empty_command(command))
		return ;
	process_arguments(command);
	if (!command->args[0])
	{
		command->exit_status = 0;
		return ;
	}
	if (is_builtin(command->args[0]))
	{
		if (is_shell_state_builtin(command->args[0]))
		{
			handle_shell_state_builtins(command);
			return ;
		}
		process_builtin_with_fork(command);
		return ;
	}
	process_external_command(command);
}
