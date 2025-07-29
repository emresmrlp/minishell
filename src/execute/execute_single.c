/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:49 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 15:27:06 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;

	if (is_builtin(command->args[0]))
	{
		execute_redirection(command);
		g_exit_status = execute_builtin(command);
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
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}
