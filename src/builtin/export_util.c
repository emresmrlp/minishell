/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 18:02:16 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 22:41:07 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**sort_array(char **env_array, int size)
{
	char	*temp;
	int		i;
	int		j;

	if (!env_array || size <= 0)
		return (NULL);
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	return (env_array);
}

int	is_valid_key(char *arg)
{
	int		key_len;
	int		i;
	char	*value;

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

char	**env_list_to_array(t_envp *env_list, int size)
{
	t_envp	*temp;
	char	*temp_str;
	char	**env_array;
	int		i;

	i = 0;
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	temp = env_list;
	while (temp)
	{
		if (temp->value)
		{
			temp_str = ft_strjoin(temp->key, "=");
			env_array[i] = ft_strjoin(temp_str, temp->value);
			free(temp_str);
		}
		else
			env_array[i] = ft_strdup(temp->key);
		i++;
		temp = temp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	set_node_values(t_envp *new_node, char *arg, char *index)
{
	new_node->value = NULL;
	new_node->is_temporary = 1;
	if (index)
	{
		new_node->key = ft_substr(arg, 0, index - arg);
		if (*(index + 1) == '\0')
			new_node->value = ft_strdup("");
		else
			new_node->value = ft_strdup(index + 1);
	}
	else
	{
		new_node->key = ft_strdup(arg);
	}
	new_node->next = NULL;
}

int	process_export_arg(t_command *command, char *arg, int i)
{
	char	*index;
	char	*key;

	if (!is_valid_key(arg))
	{
		error_handler(command, "export: not a valid identifier\n", 0);
		return (1);
	}
	index = ft_strchr(arg, '=');
	if (index)
		key = ft_substr(arg, 0, index - arg);
	else
		key = ft_strdup(arg);
	if (find_env_value(command, key))
		export_update(command, i, key);
	else
		export_add(command, i);
	free(key);
	return (0);
}
