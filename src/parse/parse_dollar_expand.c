/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:15:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 11:15:40 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*exit_status(char *str, int *i, t_envp *env_list)
{
	char	*value;

	if (str[*i] == '?')
	{
		(*i)++;
		value = get_env_value(NULL, "?", env_list);
		if (value)
			return (ft_strdup(value));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

static char	*extract_variable_name(char *str, int start, int end)
{
	char	*var_name;
	int		j;

	var_name = malloc(end - start + 1);
	if (!var_name)
		return (NULL);
	j = 0;
	while (start + j < end)
	{
		var_name[j] = str[start + j];
		j++;
	}
	var_name[j] = '\0';
	return (var_name);
}

char	*expand_var(char *str, int *i, t_envp *env_list)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*result;

	start = ++(*i);
	result = exit_status(str, i, env_list);
	if (result)
		return (result);
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;
	if (start == *i)
		return (ft_strdup("$"));
	var_name = extract_variable_name(str, start, *i);
	if (!var_name)
		return (NULL);
	value = get_env_value(NULL, var_name, env_list);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	free(var_name);
	return (result);
}
