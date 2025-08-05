/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:31 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/30 17:56:05 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	setup_pipe_and_fork(t_command **command, int *prev_fd,
		pid_t *pid, pid_t *first_pid)
{
	int	pipe_fd[2];

	if ((*command)->next != NULL && pipe(pipe_fd) == -1)
		return (perror("pipe"), exit(EXIT_FAILURE));
	else if ((*command)->next == NULL)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	*pid = fork_and_run_child(*command, *prev_fd, pipe_fd[1]);
	if (*first_pid == 0)
		*first_pid = *pid;
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	*prev_fd = pipe_fd[0];
	*command = (*command)->next;
}

void	execute_multiple(t_command *command)
{
	int			prev_fd;
	pid_t		pid;
	int			status;
	pid_t		first_pid;
	t_command	*original_command;

	original_command = command;
	prev_fd = -1;
	first_pid = 0;
	while (command)
		setup_pipe_and_fork(&command, &prev_fd, &pid, &first_pid);
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		original_command->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		original_command->exit_status = 128 + WTERMSIG(status);
	while (wait(NULL) > 0)
		;
}
