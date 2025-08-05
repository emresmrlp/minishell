/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_child_fds(int prev_fd, int write_fd)
{
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
}

void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	child_process(t_command *command, int prev_fd, int write_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_fds(prev_fd, write_fd);
	close_extra_fds();
	execute_redirection(command);
	if (is_builtin(command->args[0]))
		exit(execute_builtin(command));
	execute_command(command);
	exit(127);
}

pid_t	fork_and_run_child(t_command *command, int prev_fd, int write_fd)
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
