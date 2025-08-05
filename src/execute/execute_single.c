/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:49 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 16:08:15 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	clean_empty_args(t_command *command)
{
	int	total_args;

	total_args = 0;
	while (command->args[total_args])
		total_args++;
	remove_empty_strings(command, total_args);
	compact_args_array(command, total_args);
}

static int	handle_parent_builtins(t_command *command)
{
	if (ft_strcmp(command->args[0], "exit") == 0)
	{
		command->exit_status = builtin_exit(command, command->args);
		return (1);
	}
	if (ft_strcmp(command->args[0], "cd") == 0)
	{
		command->exit_status = builtin_cd(command);
		return (1);
	}
	if (ft_strcmp(command->args[0], "export") == 0)
	{
		command->exit_status = builtin_export(command, command->args);
		return (1);
	}
	if (ft_strcmp(command->args[0], "unset") == 0)
	{
		builtin_unset(command, command->args);
		command->exit_status = SUCCESS;
		return (1);
	}
	return (0);
}

static void	execute_child_builtins(t_command *command)
{
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;

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
	setup_child_builtin_signals_and_wait(command,
		pid, saved_stdin, saved_stdout);
}

static void	execute_external_command(t_command *command)
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

void	execute_single(t_command *command)
{
	if (handle_empty_command(command))
		return ;
	if (!command->args[0] || ft_strcmp(command->args[0], "echo") != 0)
		clean_empty_args(command);
	if (!command->args[0])
	{
		command->exit_status = 0;
		return ;
	}
	if (is_builtin(command->args[0]))
	{
		if (handle_parent_builtins(command))
			return ;
		execute_child_builtins(command);
		return ;
	}
	execute_external_command(command);
}
