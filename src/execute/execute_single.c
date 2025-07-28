/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:49 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/28 21:38:49 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//TODO setup redirectionlar ayarlanacak.
void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;

	if (is_builtin(command->args[0]))
	{
		execute_redirection(command);
		execute_builtin(command);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		execute_redirection(command);
		execute_command(command);
		exit(127);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	waitpid(pid, &status, 0);
}
