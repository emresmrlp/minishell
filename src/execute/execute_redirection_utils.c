/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redirect_input_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": No such file or directory\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	redirect_output_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": Permission denied\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	redirect_append_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": Permission denied\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

void	handle_input_redirection(t_command *command)
{
	int		i;

	if (command->heredoc_fd)
		handle_heredoc_redirection(command);
	else if (command->input_fd)
	{
		i = 0;
		while (command->input_fd[i])
		{
			if (redirect_input_simple(command->input_fd[i], command) != 0)
				exit(command->exit_status);
			i++;
		}
	}
}

char	*get_last_element(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
		i++;
	if (i > 0)
		return (array[i - 1]);
	return (NULL);
}
