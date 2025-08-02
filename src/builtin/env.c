/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:26:24 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 22:46:06 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_env_size(t_envp *env_list)
{
	int		size;
	t_envp	*temp;

	size = 0;
	temp = env_list;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}

static void	print_env(t_command *iter)
{
	t_envp	*temp;

	temp = iter->env_list;
	while (temp)
	{
		if (temp->key && temp->value)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

int	builtin_env(t_command *command)
{
	if (!command->env_list)
	{
		error_handler(command, "env: no environment variables set\n", 0);
		return (FAILURE);
	}
	print_env(command);
	return (SUCCESS);
}

void	cleanup_exit_status_str(t_command *command)
{
	get_env_value(command, "__CLEANUP__", NULL);
}

char	*handle_exit_status(t_command *command, t_envp *env_list)
{
	t_envp		*temp;
	static char	*exit_status_str = NULL;

	if (exit_status_str)
		free(exit_status_str);
	if (command)
		exit_status_str = ft_itoa(command->exit_status);
	else
	{
		temp = env_list;
		while (temp)
		{
			if (ft_strcmp(temp->key, "__EXIT_STATUS__") == 0)
			{
				exit_status_str = ft_strdup(temp->value);
				return (exit_status_str);
			}
			temp = temp->next;
		}
		exit_status_str = ft_itoa(0);
	}
	return (exit_status_str);
}
