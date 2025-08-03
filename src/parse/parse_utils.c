/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:24:39 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:39:39 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_token_split(char *str, char delimiter, char **result,
		int *params)
{
	if (str[params[3]] == '\'' && !params[2])
		params[1] = !params[1];
	else if (str[params[3]] == '"' && !params[1])
		params[2] = !params[2];
	else if (str[params[3]] == delimiter && !params[1] && !params[2])
	{
		if (params[3] > params[4])
		{
			if (params[0] >= params[5] - 1)
				return ;
			result[params[0]] = ft_substr(str, params[4],
					params[3] - params[4]);
			params[0]++;
		}
		while (str[params[3]] == delimiter)
			params[3]++;
		params[4] = params[3];
		return ;
	}
	params[3]++;
}

char	**split_with_quotes_preserved(char *str, char delimiter)
{
	char	**result;
	int		params[6];

	params[5] = calculate_buffer_size(str, delimiter);
	result = malloc(sizeof(char *) * params[5]);
	if (!result)
		return (NULL);
	params[0] = 0;
	params[1] = 0;
	params[2] = 0;
	params[3] = 0;
	params[4] = 0;
	while (str[params[3]])
		process_token_split(str, delimiter, result, params);
	if (params[3] > params[4] && params[0] < params[5] - 1)
	{
		result[params[0]] = ft_substr(str, params[4], params[3] - params[4]);
		params[0]++;
	}
	result[params[0]] = NULL;
	return (result);
}

int	validate_redirection(char **tokens, int index, t_command *command)
{
	if (!tokens[index + 1])
		return (1);
	if (is_redirection(tokens[index + 1]))
	{
		error_handler(command,
			"minishell: syntax error near unexpected token\n", 2);
		return (0);
	}
	return (1);
}
