/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:11:56 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 21:59:58 by ysumeral         ###   ########.fr       */
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

void	save_exit_status_to_env(t_envp *env_list, int exit_status)
{
	t_envp	*temp;
	char	*exit_str;

	exit_str = ft_itoa(exit_status);
	if (!exit_str)
		return ;
	
	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, "__EXIT_STATUS__") == 0)
		{
			free(temp->value);
			temp->value = exit_str;
			return ;
		}
		temp = temp->next;
	}
	
	// If not found, create new env variable
	temp = (t_envp *)malloc(sizeof(t_envp));
	if (!temp)
	{
		free(exit_str);
		return ;
	}
	temp->key = ft_strdup("__EXIT_STATUS__");
	temp->value = exit_str;
	temp->is_temporary = 1;
	temp->next = env_list->next;
	env_list->next = temp;
}
