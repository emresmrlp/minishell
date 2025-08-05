/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_helpers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_heredoc_redirection(t_command *command)
{
	int		heredoc_fd;
	int		i;

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
		{
			dup2(heredoc_fd, STDIN_FILENO);
		}
		close(heredoc_fd);
		i++;
	}
}
