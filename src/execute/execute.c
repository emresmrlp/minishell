/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:34:59 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/01 13:56:32 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(t_command *command)
{
	char	*path;
	char	**env_list_array;

	path = get_valid_path(command);
	if (!path)
		exit(g_exit_status);  // get_valid_path zaten exit code'u set etti
	env_list_array = env_list_to_array(command->env_list,
			get_env_size(command->env_list));
	if (execve(path, command->args, env_list_array) == -1)
	{
		error_handler("minishell: No such file or directory\n");
		exit(127);
	}
}

void	execute(t_command *command)
{
	if (!command)
		return ;
	if (!command->next)
		execute_single(command);
	else
		execute_multiple(command);
	// Her command execute'den sonra exit status string'ini temizle
	cleanup_exit_status_str();
}
