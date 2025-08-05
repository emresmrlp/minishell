/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:11:56 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 14:58:59 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	error_handler(t_command *command, char *message, int exit_code)
{
	if (message)
		write(2, message, ft_strlen(message));
	if (exit_code > 0)
		command->exit_status = exit_code;
	else
		command->exit_status = 2;
	return (FAILURE);
}

static void	remove_env_node(t_envp **current, t_envp **prev, t_envp **env_list)
{
	t_envp	*to_delete;

	to_delete = *current;
	if (*prev)
		(*prev)->next = (*current)->next;
	else
		*env_list = (*current)->next;
	*current = (*current)->next;
	free(to_delete->key);
	if (to_delete->value)
		free(to_delete->value);
	free(to_delete);
}

void	cleanup_empty_env_vars(t_envp **env_list)
{
	t_envp	*current;
	t_envp	*prev;

	if (!env_list || !*env_list)
		return ;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (!current->value || current->is_temporary)
			remove_env_node(&current, &prev, env_list);
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

int	shell_exit_with_error(char *message, t_command *command)
{
	if (message)
		write(2, message, ft_strlen(message));
	cleanup_exit_status_str(command);
	memory_free(command);
	exit(FAILURE);
	return (FAILURE);
}

int	shell_exit(t_command *command, int exit_code)
{
	cleanup_exit_status_str(command);
	if (command && command->env_list)
		free_env_list(command->env_list);
	memory_free(command);
	exit(exit_code);
	return (exit_code);
}
