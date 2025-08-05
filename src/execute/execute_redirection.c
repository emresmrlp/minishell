/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 21:20:47 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	heredoc_sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

static void	read_heredoc_input(t_heredoc_data *data, char *delimiter,
			t_envp *env_list)
{
	while (1)
	{
		data->line = readline("heredoc> ");
		if (!data->line)
			break ;
		if (ft_strcmp(data->line, delimiter) == 0)
		{
			free(data->line);
			break ;
		}
		data->expanded_line = expand_heredoc_line(data->line, env_list);
		write(data->temp_fd, data->expanded_line,
			ft_strlen(data->expanded_line));
		write(data->temp_fd, "\n", 1);
		free(data->line);
		free(data->expanded_line);
	}
}

static int	redirect_heredoc(char *delimiter, t_envp *env_list)
{
	t_heredoc_data	data;

	sprintf(data.temp_filename, "/tmp/minishell_heredoc_%d", getpid());
	data.temp_fd = open(data.temp_filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (data.temp_fd < 0)
		return (-1);
	data.old_sigint = signal(SIGINT, heredoc_sigint_handler);
	data.old_sigquit = signal(SIGQUIT, SIG_IGN);
	read_heredoc_input(&data, delimiter, env_list);
	lseek(data.temp_fd, 0, SEEK_SET);
	signal(SIGINT, data.old_sigint);
	signal(SIGQUIT, data.old_sigquit);
	unlink(data.temp_filename);
	return (data.temp_fd);
}

static int	create_previous_files(char **files, int is_append,
			t_command *command)
{
	t_file_creation_data	data;

	data.i = 0;
	if (!files)
		return (0);
	while (files[data.i])
	{
		if (is_append)
			data.fd = open(files[data.i], O_WRONLY | O_CREAT, 0644);
		else
			data.fd = open(files[data.i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (data.fd >= 0)
			close(data.fd);
		else
		{
			data.error_msg = ft_strjoin("minishell: ", files[data.i]);
			data.final_msg = ft_strjoin(data.error_msg, ": Permission denied\n");
			error_handler(command, data.final_msg, 1);
			free(data.error_msg);
			free(data.final_msg);
			return (1);
		}
		data.i++;
	}
	return (0);
}

void	execute_redirection(t_command *command)
{
	char	*last_output;
	char	*last_append;

	handle_input_redirection(command);
	if (command->output_fd
		&& create_previous_files(command->output_fd, 0, command) != 0)
		exit(command->exit_status);
	if (command->append_fd
		&& create_previous_files(command->append_fd, 1, command) != 0)
		exit(command->exit_status);
	last_append = get_last_element(command->append_fd);
	last_output = get_last_element(command->output_fd);
	if (last_append)
	{
		if (redirect_append_simple(last_append, command) != 0)
			exit(command->exit_status);
	}
	else if (last_output)
	{
		if (redirect_output_simple(last_output, command) != 0)
			exit(command->exit_status);
	}
}
