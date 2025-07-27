/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:39:40 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/27 14:00:26 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_unset(t_command *command, char **args)
{
	t_envp	*temp;
	int		status;
	int		i;

	status = FAILURE;
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
					status = SUCCESS;
				}
				temp = temp->next;
			}
			i++;
		}
		if (status == FAILURE)
			error_handler("unset: not a valid identifier\n");
	}
}
