/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:14:22 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/26 23:36:29 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TODO - alfabetik olarak bubble algoritmasıyla sıralanacak.

static void print_export(t_command *command)
{
	char	**env_array;
	int		i;

	i = 0;
	env_array = env_list_to_array(command->env_list);
	env_array = sort_array(env_array, get_env_size(command->env_list));
	if(!env_array)
		return ;
	while (env_array[i])
	{
		if (env_array[i])
			printf("declare -x %s\n", env_array[i]);
		i++;
	}
}

static void export_add(t_command *iter)
{
	t_envp	*temp;
	char	*index;

	temp = iter->env_list;
	while (temp->next)
		temp = temp->next;
	index = ft_strchr(iter->args[1], '=');
	if (index)
	{
		temp->key = ft_substr(iter->args[1], 0, index - iter->args[1]);
		temp->value = ft_strdup(index + 1);
		temp->next = NULL;
	}
	else
	{
		temp->key = ft_strdup(iter->args[1]);
		temp->value = NULL;
		temp->next = NULL;
	}
}

static int is_valid_key(char *arg)
{
	int key_len;
	int i;
	char *value;

	if (!arg || arg[0] == '\0')
		return (0);
	i = 1;
	value = ft_strchr(arg, '=');
	if (!value)
		key_len = ft_strlen(arg);
	else
		key_len = value - arg;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] && i < key_len)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int handle_export(t_command *command, char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		if (is_valid_key(args[i]))
			export_add(command);
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