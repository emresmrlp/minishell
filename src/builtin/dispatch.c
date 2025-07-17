/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/18 00:44:47 by ysumeral         ###   ########.fr       */
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

static int	dispatch(t_command *command)
{
	if (ft_strcmp(command->args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(command->args[0], "echo") == 0) //TODO -n opsiyonu eklenecek //FIXME PİPE VARSA EN SONUNCU ELE ALINACAK
		return (builtin_echo(command));
	else if (ft_strcmp(command->args[0], "cd") == 0)
		return (builtin_cd(command));
	else if (ft_strcmp(command->args[0], "export") == 0)
	{
		if (!command->args[1])
			print_env(command);
		else
			export_add(command);
	}
	else if (ft_strcmp(command->args[0], "unset") == 0)
		return (builtin_unset(command));
	else if (ft_strcmp(command->args[0], "env") == 0)
	   print_env(command);
	else if (ft_strcmp(command->args[0], "exit") == 0)
	{
		clear_history();
		return (shell_exit(command));
	}
	return (SUCCESS);
}

void    split_built_in(t_command *head)
{
	t_command *iter;

	iter = head;
	while (iter)
	{
		if (iter->args && iter->args[0])
		{
			dispatch(iter);
		}
		iter = iter->next;
	}
}
