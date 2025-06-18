/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/18 20:01:01 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


static char	*get_env_value(char *var_name)
{
	char	*value;

	value = getenv(var_name);
    if (!value)
    {
        return ("");
    }
	return (value);
}

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*expand_var(char *str, int *i)
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
	value = ft_strdup(get_env_value(var_name));
	free(var_name);
	return (value);
}

static char	*process_string(char *str)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	result = malloc(ft_strlen(str) * 4);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++;
		}
		else if (str[i] == '$' && !in_single && str[i + 1] && 
				is_var_char(str[i + 1]))
		{
			temp = expand_var(str, &i);
			if (temp)
			{
				ft_strlcpy(result + j, temp, ft_strlen(temp) + 1);
				j += ft_strlen(temp);
				free(temp);
			}
		}
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
		else if (str[i] == '$' && !in_single && str[i + 1] && 
				is_var_char(str[i + 1]))
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
			processed = process_string(iter->args[i]);
			if (processed)
			{
				free(iter->args[i]);
				iter->args[i] = processed;
			}
			i++;
		}
		iter = iter->next;
	}
    print_command_list(head);
}