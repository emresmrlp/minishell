/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	heredoc_sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

static int	setup_heredoc_file(char *temp_filename)
{
	int	temp_fd;

	create_temp_filename(temp_filename);
	temp_fd = open(temp_filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	return (temp_fd);
}

static void	process_heredoc_line(char *line, int temp_fd, t_envp *env_list)
{
	char	*expanded_line;

	expanded_line = expand_heredoc_line(line, env_list);
	write(temp_fd, expanded_line, ft_strlen(expanded_line));
	write(temp_fd, "\n", 1);
	free(expanded_line);
}

static void	read_heredoc_input(char *delimiter, int temp_fd, t_envp *env_list)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		process_heredoc_line(line, temp_fd, env_list);
		free(line);
	}
}

int	redirect_heredoc(char *delimiter, t_envp *env_list)
{
	int		temp_fd;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);
	char	temp_filename[256];

	temp_fd = setup_heredoc_file(temp_filename);
	if (temp_fd < 0)
		return (-1);
	old_sigint = signal(SIGINT, heredoc_sigint_handler);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	read_heredoc_input(delimiter, temp_fd, env_list);
	close(temp_fd);
	temp_fd = open(temp_filename, O_RDONLY);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	unlink(temp_filename);
	return (temp_fd);
}
