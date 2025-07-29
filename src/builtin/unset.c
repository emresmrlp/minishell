/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:39:40 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 15:52:41 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_unset(t_command *command, char **args)
{
	t_envp	*temp;
	int		i;

	if (args[1] != NULL)
	{
		i = 1;
		while (args[i])
		{
			temp = command->env_list;
			while (temp)
			{
				if (ft_strcmp(temp->key, args[i]) == 0)
				{
					remove_env(command, args[i]);
					break;
				}
				temp = temp->next;
			}
			i++;
		}
	}
}
