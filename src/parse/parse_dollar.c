/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 17:37:36 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*process_string(char *str, t_envp *env_list, int i, int j)
{
	char	*result;
	int		in_single;
	int		in_double;

	result = malloc(500);
	if (!result)
		return (NULL);
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

static void	process_argument(t_command *iter, int i)
{
	char	*processed;

	if (!iter->args[i])
		return ;
	if (iter->skip_expansion && iter->skip_expansion[i])
		return ;
	iter->dollar = has_expansion(iter->args[i]);
	if(!iter->args[i])
		processed = NULL;
	else
		processed = process_string(iter->args[i], iter->env_list, 0, 0);
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
