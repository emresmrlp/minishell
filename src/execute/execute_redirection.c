/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 14:55:55 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	redirect_input(t_command *command)
{
	int	fd;

	fd = open(command->input_fd, O_RDONLY);
	if (fd < 0)
	{
		perror("open input");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 input");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static void	redirect_output(t_command *command)
{
	int	fd;

	fd = open(command->output_fd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open output");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 output");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static void	redirect_append(t_command *command)
{
	int	fd;

	fd = open(command->append_fd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open append");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 append");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static int	redirect_heredoc(char *delimiter)
{
	int		heredoc_fd[2];
	char	*line;

	pipe(heredoc_fd);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(heredoc_fd[1]);
			break ;
		}
		write(heredoc_fd[1], line, ft_strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}

void	execute_redirection(t_command *command)
{
	int	heredoc_fd;

	if (command->input_fd)
		redirect_input(command);
	if (command->output_fd)
		redirect_output(command);
	if (command->append_fd)
		redirect_append(command);
	if (command->heredoc_fd)
	{
		heredoc_fd = redirect_heredoc(command->heredoc_fd);
		if (heredoc_fd == -1)
		{
			perror("heredoc");
			exit(EXIT_FAILURE);
		}
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
}
