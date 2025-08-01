/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/01 15:26:48 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	has_expansion(char *str);

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*my_strdup(char *src)
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

static char	*expand_var(char *str, int *i, t_envp *env_list)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*result;
	int		j;

	start = ++(*i);
	if (str[*i] == '?')
	{
		(*i)++;
		value = get_env_value("?", env_list);
		return (my_strdup(value));
	}
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;
	if (start == *i)
		return (my_strdup("$"));
	var_name = malloc(*i - start + 1);
	if (!var_name)
		return (NULL);
	j = 0;
	while (start < *i)
		var_name[j++] = str[start++];
	var_name[j] = '\0';
	value = get_env_value(var_name, env_list);
	if (!value)
	{
		free(var_name);
		return (my_strdup(""));
	}
	result = my_strdup(value);
	free(var_name);
	return (result);
}

static void	handle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static void	append_expansion(char **result, int *j, char *str, int *i, 
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

static char	*process_string(char *str, t_envp *env_list)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	if (!str)
		return (NULL);
	result = malloc(500);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			handle_quotes(str[i], &in_single, &in_double);
			result[j++] = str[i++];
		}
		else if (str[i] == '$' && !in_single
			&& str[i + 1] && (is_var_char(str[i + 1]) || str[i + 1] == '?'))
			append_expansion(&result, &j, str, &i, env_list, 500);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	has_expansion(char *str)
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

static void	process_argument(t_command *iter, int i)
{
	char	*processed;

	if (!iter->args[i])
		return ;
	if (iter->skip_expansion && iter->skip_expansion[i])
		return ;
	iter->dollar = has_expansion(iter->args[i]);
	processed = process_string(iter->args[i], iter->env_list);
	if (processed)
	{
		free(iter->args[i]);
		iter->args[i] = processed;
	}
}

void	parse_dollar(t_command *head)
{
	t_command	*iter;
	int			i;

	iter = head;
	while (iter)
	{
		i = 0;
		while (iter->args && iter->args[i])
		{
			process_argument(iter, i);
			i++;
		}
		iter = iter->next;
	}
}
