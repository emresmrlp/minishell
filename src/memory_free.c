/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:32:16 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 17:08:32 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static void	free_env_list(t_envp *env_list) //  ana döngüde readline ın orada
// {
// 	t_envp	*temp;

// 	while (env_list)
// 	{
// 		temp = env_list;
// 		env_list = env_list->next;
// 		if (temp->key)
// 			free(temp->key);
// 		if (temp->value)
// 			free(temp->value);
// 		free(temp);
// 	}
// }

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
        //free(temp->env_list->key);
        //free(temp->env_list->value);
        //free(temp->env_list);
		free_command_node(temp);
		free(temp);
	}
}
