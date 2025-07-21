/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:34:59 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/21 13:57:09 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(command->args[0], command->args) == -1)
			printf("minishell: command not found\n");
		exit(127);
	}
	else
		waitpid(pid, &status, 0);
}

/*static void execute_pipeline(t_command *command)
{
	(void)command;
}*/

void	execute(t_command *command)
{
	if (!command)
		return ;
	if (!command->next)
	{
		if (handle_builtin(command) == SUCCESS)
			return ;
		else
			execute_single(command);
		return ;
	}
	else
	{
		handle_builtin(command);
	}
}
