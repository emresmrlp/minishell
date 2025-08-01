/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:09:52 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/01 14:43:35 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_envp	*env_head(char **envp)
{
	t_envp	*temp;
	t_envp	*head;
	int		i;

	i = 0;
	head = (t_envp *)malloc(sizeof(t_envp));
	if (!head)
		return (NULL);
	temp = head;
	while (envp[i])
	{
		temp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		temp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		temp->is_temporary = 0;  // Başlangıç environment'ı kalıcı
		if (envp[i + 1])
		{
			temp->next = (t_envp *)malloc(sizeof(t_envp));
			if (!temp->next)
				return (NULL);
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
		{
			if (temp->value)
				return (temp->value);
			else
				return ("");  // Boş string literal döndür, malloc gerektirmez
		}
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
	new_node->is_temporary = 1;  // Yeni eklenen variable'lar temporary
	new_node->next = iter->env_list;
	iter->env_list = new_node;
}

void	remove_env(t_command *command, char *key)
{
	t_envp	*temp;
	t_envp	*prev;

	temp = command->env_list;
	prev = NULL;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				command->env_list = temp->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

char	*get_env_value(char *var_name, t_envp *env_list)
{
	t_envp	*temp;
	static char	*exit_status_str = NULL;

	// Cleanup çağrısı
	if (var_name && ft_strcmp(var_name, "__CLEANUP__") == 0)
	{
		if (exit_status_str)
		{
			free(exit_status_str);
			exit_status_str = NULL;
		}
		return (NULL);
	}
	
	if (ft_strcmp(var_name, "?") == 0)
	{
		if (exit_status_str)
			free(exit_status_str);
		exit_status_str = ft_itoa(g_exit_status);
		return (exit_status_str);
	}
	temp = env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, var_name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return ("");
}

void	cleanup_exit_status_str(void)
{
	// get_env_value fonksiyonunu özel parametre ile çağırarak cleanup yapıyoruz
	// Her çağrıda cleanup yapılabilir, tekrar çağırma koruması olmayacak
	get_env_value("__CLEANUP__", NULL);
}
