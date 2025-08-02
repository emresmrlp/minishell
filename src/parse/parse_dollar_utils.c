/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:28:14 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 21:44:05 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*my_strdup(char *src)
{
	char	*dst;
	int		len;
	int		i;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static char	*handle_special_var(char *str, int *i, t_envp *env_list)
{
	char	*value;

	if (str[*i] == '?')
	{
		(*i)++;
		value = get_env_value(NULL, "?", env_list);
		return (my_strdup(value));
	}
	return (NULL);
}

static char	*extract_var_name(char *str, int *i, int start)
{
	char	*var_name;
	int		j;

	if (start == *i)
		return (NULL);
	var_name = malloc(*i - start + 1);
	if (!var_name)
		return (NULL);
	j = 0;
	while (start < *i)
		var_name[j++] = str[start++];
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
	result = handle_special_var(str, i, env_list);
	if (result)
		return (result);
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;
	if (start == *i)
		return (my_strdup("$"));
	var_name = extract_var_name(str, i, start);
	if (!var_name)
		return (NULL);
	value = get_env_value(NULL, var_name, env_list);
	if (!value)
	{
		free(var_name);
		return (my_strdup(""));
	}
	result = my_strdup(value);
	free(var_name);
	return (result);
}

void	handle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

void	append_expansion(char **result, int *j, char *str, int *i, 
		t_envp *env_list, int buffer_size)
{
	char	*temp;
	int		len;
	int		k;

	temp = expand_var(str, i, env_list);
	if (temp)
	{
		len = ft_strlen(temp);
		k = 0;
		while (k < len && *j < buffer_size - 1)
		{
			(*result)[*j] = temp[k];
			(*j)++;
			k++;
		}
		free(temp);
	}
}

int	has_expansion(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '$' && !in_single && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
			return (1);
		i++;
	}
	return (0);
}
