/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:09:52 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/13 12:53:36 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_envp	*env_head(char **envp)
{
	t_envp	*temp;
	t_envp	*head;
	int		i;

	i = 0;
	head = (t_envp *)malloc(sizeof(t_envp));
	head->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
	head->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
	temp = head;
	while (envp[i])
	{
		temp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		temp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (envp[i + 1])
		{
			temp->next = (t_envp *)malloc(sizeof(t_envp));
			if (!temp->next)
				error("Malloc Error\n");
			temp = temp->next;
		}
		else
			temp->next = NULL;
		i++;
	}
	return (head);
}

char	*find_env_value(t_command *iter, char *key)
{
	t_envp	*temp;

	temp = iter->env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

void	update_env(t_command *iter, char *key, char *new_value)
{
	t_envp	*temp;
	t_envp	*new_node;

	temp = iter->env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(new_value);
			return ;
		}
		temp = temp->next;
	}
	new_node = (t_envp *)malloc(sizeof(t_envp));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(new_value);
	new_node->next = iter->env_list;
	iter->env_list = new_node;
}

char	*get_env_value(char *var_name, t_envp *env_list)
{
	t_envp	*temp;

	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, var_name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return ("");
}