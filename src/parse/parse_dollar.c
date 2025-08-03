/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:30:25 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_expanded_variable(char *str, int *i, t_envp *env_list)
{
	return (expand_var(str, i, env_list));
}

static void	process_char(char *str, t_envp *env_list, t_parse_data *data)
{
	char	*temp;

	if ((str[*(data->i)] == '\'' || str[*(data->i)] == '"'))
	{
		handle_quotes(str[*(data->i)], &data->state[0], &data->state[1]);
		data->result[(*(data->j))++] = str[(*(data->i))++];
	}
	else if (str[*(data->i)] == '$' && !data->state[0] && str[*(data->i) + 1]
		&& (is_var_char(str[*(data->i) + 1]) || str[*(data->i) + 1] == '?'))
	{
		temp = get_expanded_variable(str, data->i, env_list);
		if (temp)
			append_text_to_result(&data->result, data->j, temp);
		free(temp);
	}
	else
		data->result[(*(data->j))++] = str[(*(data->i))++];
}

static char	*process_string(char *str, t_envp *env_list, int i, int j)
{
	char			*result;
	int				state[2];
	t_parse_data	data;

	result = malloc(500);
	if (!result)
		return (NULL);
	state[0] = 0;
	state[1] = 0;
	data.result = result;
	data.i = &i;
	data.j = &j;
	data.state = state;
	while (str[i])
		process_char(str, env_list, &data);
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
	iter->dollar = expansion_check(iter->args[i]);
	if (!iter->args[i])
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
