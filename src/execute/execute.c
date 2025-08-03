/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:34:59 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 17:54:28 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(t_command *command)
{
	char	*path;
	char	**env_list_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	path = get_valid_path(command);
	if (!path)
		exit(command->exit_status);
	env_list_array = env_list_to_array(command->env_list,
			get_env_size(command->env_list));
	if (execve(path, command->args, env_list_array) == -1)
		exit(127);
}

void	execute(t_command *command)
{
	if (!command)
		return ;
	if (!command->next)
		execute_single(command);
	else
		execute_multiple(command);
	save_exit_status_to_env(command->env_list, command->exit_status);
	cleanup_exit_status_str(command);
}
