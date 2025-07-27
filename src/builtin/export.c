/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:14:22 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/27 12:58:24 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TODO - alfabetik olarak bubble algoritmasıyla sıralanacak.

static void print_export(t_command *command)
{
	char	**env_array;
	int		i;

	i = 0;
	env_array = env_list_to_array(command->env_list,
		get_env_size(command->env_list));
	if (!env_array)
	{
		printf("hatata\n");
		return ;
	}
	env_array = sort_array(env_array, get_env_size(command->env_list));
	if(!env_array)
	{
		printf("hatata\n");
		return ;
	}
	while (env_array[i])
	{
		if (env_array[i])
			printf("declare -x %s\n", env_array[i]);
		i++;
	}
}

static void export_add(t_command *iter, int arg_index)
{
	t_envp	*temp;
	t_envp	*new_node;
	char	*index;

	temp = iter->env_list;
	while (temp->next)
		temp = temp->next;
	index = ft_strchr(iter->args[arg_index], '=');
	new_node = malloc(sizeof(t_envp));
	if (!new_node)
		return ;
	new_node->value = NULL;
	if (index)
	{
		new_node->key = ft_substr(iter->args[arg_index], 0,
			index - iter->args[arg_index]);
		if (*(index + 1) == '\0')
			new_node->value = ft_strdup("");
		else
			new_node->value = ft_strdup(index + 1);
	}
	else
		new_node->key = ft_strdup(iter->args[arg_index]);
	new_node->next = NULL;
	temp->next = new_node;
}

static void export_update(t_command *iter, int arg_index, char *key)
{
	t_envp *temp;
	char *index;

	temp = iter->env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			index = ft_strchr(iter->args[arg_index], '=');
			if (!index)
				return ;
			free(temp->value);
			temp->value = NULL;
			if (index)
			{
				if (*(index + 1) == '\0')
					temp->value = ft_strdup("");
				else
					temp->value = ft_strdup(index + 1);
			}
			return ;
		}
		temp = temp->next;
	}
}

static int handle_export(t_command *command, char **args)
{
	int i;
	char *index;
	char *key;

	i = 1;
	while (args[i])
	{
		index = ft_strchr(command->args[i], '=');
		key = ft_substr(command->args[i], 0, index - command->args[i]);
		if (is_valid_key(key))
		{
			if (find_env_value(command, key))
			{
				export_update(command, i, key);
				printf("%s updated.\n", args[i]);
			}
			else
			{
				export_add(command, i);
				printf("%s added.\n", args[i]);
			}
		}
		else
			error_handler("export: not a valid identifier\n");
		i++;
	}
	return (SUCCESS);
}

int builtin_export(t_command *command, char **args)
{
	if (args[1] == NULL)
	{
		print_export(command);
		return (SUCCESS);
	}
	if (handle_export(command, args))
		return (SUCCESS);
	else
		return (FAILURE);
}