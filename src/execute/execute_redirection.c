/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/30 16:46:01 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	redirect_input(t_command *command)
{
	int	fd;

	fd = open(command->input_fd, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell");
		return (1);  // Error, but don't exit
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 input");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_output(t_command *command)
{
	int	fd;

	fd = open(command->output_fd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);  // Error, but don't exit
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 output");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_append(t_command *command)
{
	int	fd;

	fd = open(command->append_fd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);  // Error, but don't exit
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 append");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static void	heredoc_sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	// Exit the readline call
	exit(130);
}

static int	redirect_heredoc(char *delimiter)
{
	int		heredoc_fd[2];
	char	*line;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	pipe(heredoc_fd);
	
	// Setup signal handlers
	old_sigint = signal(SIGINT, heredoc_sigint_handler);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	
	while (1)
	{
		line = readline("heredoc> ");
		
		// Check for EOF (Ctrl-D)
		if (!line)
		{
			break;
		}
		
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(heredoc_fd[1], line, ft_strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
	
	// Restore original signal handlers
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	
	return (heredoc_fd[0]);
}

void	execute_redirection(t_command *command)
{
	int	heredoc_fd;
	int	error_occurred = 0;

	// Heredoc önce işlenmeli - stdin'i hazırlar
	if (command->heredoc_fd)
	{
		heredoc_fd = redirect_heredoc(command->heredoc_fd);
		if (heredoc_fd == -1)
		{
			g_exit_status = 130;  // Standard exit code for SIGINT/Ctrl-D
			return;
		}
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	else if (command->input_fd)
	{
		if (redirect_input(command) != 0)
			error_occurred = 1;
	}
	
	// Multiple output redirections: en son olan geçerli olur
	if (command->append_fd)
	{
		if (redirect_append(command) != 0)
			error_occurred = 1;
	}
	else if (command->output_fd)
	{
		if (redirect_output(command) != 0)
			error_occurred = 1;
	}
	
	// Eğer redirection error'ı varsa exit code 1 ile çık
	if (error_occurred)
		exit(1);
}
