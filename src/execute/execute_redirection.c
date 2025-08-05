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
