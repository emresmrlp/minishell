/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:01:54 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/26 18:11:49 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_env(t_envp *head, char *search)
{
	t_envp	*temp;

	temp = head;
	while (temp)
	{
		if (ft_strcmp(temp->key, search) == 0)
		{
			free(temp->key);
			return (temp->value);
		}
		temp = temp->next;
	}
	return (NULL);
}

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

void	print_env(t_command *iter)
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
