/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_empty_strings(t_command *command, int total_args)
{
	int	i;

	i = 0;
	while (i < total_args)
	{
		if (command->args[i] && command->args[i][0] == '\0')
		{
			free(command->args[i]);
			command->args[i] = NULL;
		}
		i++;
	}
}

void	compact_args_array(t_command *command, int total_args)
{
	int	i;
	int	write_idx;

	write_idx = 0;
	i = 0;
	while (i < total_args)
	{
		if (command->args[i] != NULL)
		{
			if (i != write_idx)
				command->args[write_idx] = command->args[i];
			write_idx++;
		}
		i++;
	}
	command->args[write_idx] = NULL;
}

void	setup_child_builtin_signals_and_wait(t_command *command,
		pid_t pid, int saved_stdin, int saved_stdout)
{
	int	status;

	g_signal_flag = 1;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	g_signal_flag = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		command->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		command->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	setup_external_signals_and_wait(t_command *command, pid_t pid)
{
	int	status;

	g_signal_flag = 1;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	g_signal_flag = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		command->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		command->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
}

int	handle_empty_command(t_command *command)
{
	if (!command->args || !command->args[0])
	{
		if (command->heredoc_fd)
		{
			execute_redirection(command);
			command->exit_status = SUCCESS;
			return (1);
		}
		else
		{
			error_handler(command,
				"minishell: syntax error near unexpected token\n", 0);
			return (1);
		}
	}
	return (0);
}
