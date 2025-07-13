/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/13 12:50:44 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*expand_var(char *str, int *i, t_envp *env_list)
{
	int		start;
	char	*var_name;
	char	*value;
	int		j;

	start = ++(*i);
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;
	if (start == *i)
		return (ft_strdup("$"));
	var_name = malloc(*i - start + 1);
	if (!var_name)
		return (NULL);
	j = 0;
	while (start < *i)
		var_name[j++] = str[start++];
	var_name[j] = '\0';
	value = ft_strdup(get_env_value(var_name, env_list));
	free(var_name);
	return (value);
}

static void	handle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static void	append_expansion(char **result, int *j, char *str, int *i, t_envp *env_list)
{
	char	*temp;

	temp = expand_var(str, i, env_list);
	if (temp)
	{
		ft_strlcpy(*result + *j, temp, ft_strlen(temp) + 1);
		*j += ft_strlen(temp);
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

	result = malloc(ft_strlen(str) * 4);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			handle_quotes(str[i], &in_single, &in_double);
			i++;
		}
		else if (str[i] == '$' && !in_single
			&& str[i + 1] && is_var_char(str[i + 1]))
			append_expansion(&result, &j, str, &i, env_list);
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

	i = 0;
	in_single = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !ft_strchr(str, '"'))
			in_single = !in_single;
		else if (str[i] == '$' && !in_single && str[i + 1]
			&& is_var_char(str[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

void	parse_dollar(t_command *head)
{
	t_command	*iter;
	char		*processed;
	int			i;

	iter = head;
	while (iter)
	{
		i = 0;
		while (iter->args && iter->args[i])
		{
			iter->dollar = has_expansion(iter->args[i]);
			processed = process_string(iter->args[i], iter->env_list);
			if (processed)
			{
				free(iter->args[i]);
				iter->args[i] = processed;
			}
			i++;
		}
		iter = iter->next;
	}
	split_built_in(head);
	print_command_list(head);
}
