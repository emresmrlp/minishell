/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:01:54 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/17 18:59:10 by ysumeral         ###   ########.fr       */
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
