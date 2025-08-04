/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:34:59 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 21:08:15 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(t_command *command)
{
	char	*path;
	char	**env_list_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	path = get_valid_path(command);
	if (!path)
		exit(command->exit_status);
	env_list_array = env_list_to_array(command->env_list,
			get_env_size(command->env_list));
	if (execve(path, command->args, env_list_array) == -1)
		exit(127);
}

void	execute(t_command *command)
{
	pid_t	pid;
	int		status;

	if (!command)
		return ;
	if (command->argc == 0)
	{
		if (command->input_fd || command->output_fd
			|| command->append_fd || command->heredoc_fd)
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				execute_redirection(command);
				exit(command->exit_status);
			}
			else if (pid < 0)
			{
				command->exit_status = 1;
				return ;
			}
			else
			{
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
					command->exit_status = 128 + WTERMSIG(status);
			}
			save_exit_status_to_env(command->env_list, command->exit_status);
			cleanup_exit_status_str(command);
		}
		else
		{
			command->exit_status = 0;
		}
		return ;
	}
	if (!command->next)
		execute_single(command);
	else
		execute_multiple(command);
	save_exit_status_to_env(command->env_list, command->exit_status);
	cleanup_exit_status_str(command);
}
