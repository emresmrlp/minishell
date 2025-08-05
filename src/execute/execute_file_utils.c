/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	create_single_file(char *filename, int is_append,
	t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	if (is_append)
		fd = open(filename, O_WRONLY | O_CREAT, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		close(fd);
		return (0);
	}
	error_msg = ft_strjoin("minishell: ", filename);
	final_msg = ft_strjoin(error_msg, ": Permission denied\n");
	error_handler(command, final_msg, 1);
	free(error_msg);
	free(final_msg);
	return (1);
}

int	create_previous_files(char **files, int is_append, t_command *command)
{
	int	i;

	if (!files)
		return (0);
	i = 0;
	while (files[i])
	{
		if (create_single_file(files[i], is_append, command) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	process_heredoc_fds(t_command *command)
{
	int	heredoc_fd;
	int	i;

	i = 0;
	while (command->heredoc_fd[i])
	{
		heredoc_fd = redirect_heredoc(command->heredoc_fd[i],
				command->env_list);
		if (heredoc_fd == -1)
		{
			command->exit_status = 130;
			return ;
		}
		if (!command->heredoc_fd[i + 1])
			dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
		i++;
	}
}

static void	process_input_fds(t_command *command)
{
	int	i;

	if (!command->input_fd)
		return ;
	i = 0;
	while (command->input_fd[i])
	{
		if (redirect_input_simple(command->input_fd[i], command) != 0)
			exit(command->exit_status);
		i++;
	}
}

void	handle_input_redirection(t_command *command)
{
	if (command->heredoc_fd)
		process_heredoc_fds(command);
	else
		process_input_fds(command);
}
