/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:11:56 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/01 14:43:36 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	error_handler(char *message)
{
	if (message)
		write(2, message, ft_strlen(message));
	g_exit_status = 2;
	return (FAILURE);
}

void	free_env_list(t_envp *env_list)
{
	t_envp	*temp;

	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void	cleanup_empty_env_vars(t_envp **env_list)
{
	t_envp	*current;
	t_envp	*prev;
	t_envp	*to_delete;

	if (!env_list || !*env_list)
		return ;

	current = *env_list;
	prev = NULL;

	while (current)
	{
		// Temporary variable'ları temizle (value yoksa veya temporary ise)
		if (!current->value || current->is_temporary)
		{
			to_delete = current;
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			
			current = current->next;
			free(to_delete->key);
			if (to_delete->value)
				free(to_delete->value);
			free(to_delete);
		}
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
	cleanup_exit_status_str();
	memory_free(command);
	exit(FAILURE);
	return (FAILURE);
}

int	shell_exit(t_command *command, int exit_code)
{
	cleanup_exit_status_str();
	if (command && command->env_list)
		free_env_list(command->env_list);
	memory_free(command);
	exit(exit_code);
	return (exit_code);
}
