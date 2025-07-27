/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:26:24 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/27 14:07:07 by ysumeral         ###   ########.fr       */
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
		error_handler("env: no environment variables set\n");
		return (FAILURE);
	}
	print_env(command);
	return (SUCCESS);
}
