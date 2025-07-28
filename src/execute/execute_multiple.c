/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:31 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/28 22:21:26 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	child_process(t_command *command, int prev_fd, int write_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (write_fd != -1)
	{
		dup2(write_fd, STDOUT_FILENO);
		close(write_fd);
	}
	execute_redirection(command);
	if (is_builtin(command->args[0]))
		exit(execute_builtin(command));
	execute_command(command);
	exit(127);
}

static pid_t	fork_and_run_child(t_command *command,
	int prev_fd, int write_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		child_process(command, prev_fd, write_fd);
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	execute_multiple(t_command *command)
{
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;

	prev_fd = -1;
	while (command)
	{
		if (command->next != NULL && pipe(pipe_fd) == -1)
			return (perror("pipe"), exit(EXIT_FAILURE));
		else if (command->next == NULL)
		{
			pipe_fd[0] = -1;
			pipe_fd[1] = -1;
		}
		pid = fork_and_run_child(command, prev_fd, pipe_fd[1]);
		if (prev_fd != -1)
			close(prev_fd);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
		command = command->next;
	}
	while (wait(NULL) > 0)
		(void)0;
}
