/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/01 12:42:13 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	redirect_input_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
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

static int	redirect_output_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
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

static int	redirect_append_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
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

// Son elemanı bul (array'in sonuncusu aktif redirection)
static char *get_last_element(char **array)
{
	int i = 0;
	
	if (!array)
		return NULL;
	
	while (array[i])
		i++;
	
	if (i > 0)
		return array[i - 1];
	
	return NULL;
}

// Array'daki son eleman hariç tüm dosyaları oluştur
static void create_previous_files(char **files, int is_append)
{
	int i = 0;
	int fd;
	
	if (!files)
		return;
	
	// Son eleman hariç tüm dosyaları oluştur
	while (files[i + 1])  // Son eleman (NULL) değilse devam et
	{
		if (is_append)
			fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		else
			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd >= 0)
			close(fd);
		else
			perror("minishell");
		i++;
	}
}

void	execute_redirection(t_command *command)
{
	int	heredoc_fd;
	int	i;
	char *last_input, *last_output, *last_append;

	// Heredoc'ları sırayla işle (tüm heredoc'ları consume et, son olanını kullan)
	if (command->heredoc_fds)
	{
		i = 0;
		while (command->heredoc_fds[i])
		{
			heredoc_fd = redirect_heredoc(command->heredoc_fds[i]);
			if (heredoc_fd == -1)
			{
				g_exit_status = 130;
				return;
			}
			// Sadece son heredoc'u stdin'e yönlendir
			if (!command->heredoc_fds[i + 1])  // Son heredoc
			{
				dup2(heredoc_fd, STDIN_FILENO);
			}
			close(heredoc_fd);
			i++;
		}
	}
	else
	{
		// Son input dosyasını kullan
		last_input = get_last_element(command->input_fds);
		if (last_input && redirect_input_simple(last_input) != 0)
			exit(1);
	}
	
	// Önceki dosyaları boş oluştur
	create_previous_files(command->output_fds, 0);  // Output files (truncate)
	create_previous_files(command->append_fds, 1);  // Append files (no truncate)
	
	// Son redirection'ı aktif olarak kullan
	last_append = get_last_element(command->append_fds);
	last_output = get_last_element(command->output_fds);
	
	if (last_append)
	{
		if (redirect_append_simple(last_append) != 0)
			exit(1);
	}
	else if (last_output)
	{
		if (redirect_output_simple(last_output) != 0)
			exit(1);
	}
}
