/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:32:16 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 14:40:59 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_command_node(t_command *cmd)
{
	if (!cmd)
		return ;
	free_command_args(cmd->args);
	if (cmd->input_fd)
		free_command_args(cmd->input_fd);
	if (cmd->output_fd)
		free_command_args(cmd->output_fd);
	if (cmd->append_fd)
		free_command_args(cmd->append_fd);
	if (cmd->heredoc_fd)
		free_command_args(cmd->heredoc_fd);
	if (cmd->skip_expansion)
		free(cmd->skip_expansion);
}

void	memory_free(t_command *command)
{
	t_command	*temp;

	if (!command)
		return ;
	while (command)
	{
		temp = command;
		command = command->next;
		free_command_node(temp);
		free(temp);
	}
}
