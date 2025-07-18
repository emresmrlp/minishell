/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:14:22 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 12:46:22 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void export_add(t_command *iter)
{
	t_envp	*temp;
	char	*index;

	temp = iter->env_list;
	while (temp->next)
		temp = temp->next;
	temp->next = (t_envp *)malloc(sizeof(t_envp));
	temp->next->next = NULL;
	index = ft_strchr(iter->args[1], '=');
	printf("%s\n", iter->args[1]);
	if (index == 0)
	{
		temp->next->key = ft_strdup(iter->args[1]);
		temp->next->value = NULL;
		return;
	}
	if (index)
	{
		temp->next->key = ft_substr(iter->args[1], 0, index - iter->args[1]);
		temp->next->value = ft_strdup(index + 1);
	}
	else
	{
		temp->next->key = ft_strdup(iter->args[1]);
		temp->next->value = NULL;
	}
}

int builtin_export(t_command *command)
{
    if (!command->args[1])
    {
        print_env(command);
        return (SUCCESS);
    }
    export_add(command);
    return (SUCCESS);
}