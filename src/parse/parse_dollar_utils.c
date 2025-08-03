/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:28:14 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:34:32 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	handle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static void	str_copy(char **result, int *j, char *temp)
{
	int		buffer_size;
	int		len;
	int		k;

	buffer_size = 500;
	len = ft_strlen(temp);
	k = 0;
	while (k < len && *j < buffer_size - 1)
	{
		(*result)[*j] = temp[k];
		(*j)++;
		k++;
	}
}

void	append_text_to_result(char **result, int *j, char *text)
{
	str_copy(result, j, text);
}

int	expansion_check(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		handle_quotes(str[i], &in_single, &in_double);
		if (str[i] == '$' && !in_single && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
			return (1);
		i++;
	}
	return (0);
}
