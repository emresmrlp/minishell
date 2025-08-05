/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_variable(t_expansion_data *data, char *line,
	t_envp *env_list)
{
	data->j = data->i + 1;
	while (line[data->j] && (ft_isalnum(line[data->j])
			|| line[data->j] == '_'))
		data->j++;
	data->temp = ft_substr(line, data->i + 1,
			data->j - data->i - 1);
	if (data->temp)
	{
		data->env_value = get_env_value(NULL, data->temp, env_list);
		data->old_result = data->result;
		if (data->env_value)
			data->result = ft_strjoin(data->result, data->env_value);
		else
			data->result = ft_strjoin(data->result, "");
		free(data->old_result);
		free(data->temp);
	}
	data->i = data->j;
}

char	*expand_heredoc_line(char *line, t_envp *env_list)
{
	t_expansion_data	data;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	data.result = ft_strdup("");
	data.i = 0;
	while (line[data.i])
	{
		if (line[data.i] == '$' && line[data.i + 1]
			&& (ft_isalnum(line[data.i + 1])
				|| line[data.i + 1] == '_'))
			expand_variable(&data, line, env_list);
		else
		{
			data.temp = ft_substr(line, data.i, 1);
			data.old_result = data.result;
			data.result = ft_strjoin(data.result, data.temp);
			free(data.old_result);
			free(data.temp);
			data.i++;
		}
	}
	return (data.result);
}
