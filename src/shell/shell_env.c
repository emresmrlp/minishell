/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:50:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:58:55 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_envp	*create_exit_status_node(int exit_status)
{
	t_envp	*temp;
	char	*exit_str;

	exit_str = ft_itoa(exit_status);
	if (!exit_str)
		return (NULL);
	temp = (t_envp *)malloc(sizeof(t_envp));
	if (!temp)
	{
		free(exit_str);
		return (NULL);
	}
	temp->key = ft_strdup("__EXIT_STATUS__");
	temp->value = exit_str;
	temp->is_temporary = 1;
	temp->next = NULL;
	return (temp);
}

void	save_exit_status_to_env(t_envp *env_list, int exit_status)
{
	t_envp	*temp;
	t_envp	*new_node;
	char	*exit_str;

	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, "__EXIT_STATUS__") == 0)
		{
			exit_str = ft_itoa(exit_status);
			if (!exit_str)
				return ;
			free(temp->value);
			temp->value = exit_str;
			return ;
		}
		temp = temp->next;
	}
	new_node = create_exit_status_node(exit_status);
	if (new_node)
	{
		new_node->next = env_list->next;
		env_list->next = new_node;
	}
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
