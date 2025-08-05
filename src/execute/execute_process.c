/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_builtin_with_fork(t_command *command)
{
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_redirection(command);
		exit(execute_builtin(command));
	}
	else if (pid < 0)
	{
		perror("fork");
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	setup_child_builtin_signals_and_wait(command, pid, saved_stdin,
		saved_stdout);
}

void	process_external_command(t_command *command)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_redirection(command);
		execute_command(command);
		exit(127);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	setup_external_signals_and_wait(command, pid);
}

void	handle_shell_state_builtins(t_command *command)
{
	if (ft_strcmp(command->args[0], "exit") == 0)
	{
		command->exit_status = builtin_exit(command, command->args);
		return ;
	}
	if (ft_strcmp(command->args[0], "cd") == 0)
	{
		command->exit_status = builtin_cd(command);
		return ;
	}
	if (ft_strcmp(command->args[0], "export") == 0)
	{
		command->exit_status = builtin_export(command, command->args);
		return ;
	}
	if (ft_strcmp(command->args[0], "unset") == 0)
	{
		builtin_unset(command, command->args);
		command->exit_status = SUCCESS;
		return ;
	}
}
