/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_function_file.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:30:34 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:30:28 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_redirection_token(t_command *node, char **tokens,
		char **original_tokens, int *index)
{
	handle_redirection(node, tokens, &index[0]);
	if (!is_token_quoted(original_tokens, index[2]))
	{
		index[2] += 2;
		index[0]++;
		return ;
	}
	node->args[index[1]] = ft_strdup(tokens[index[0]]);
	if (original_tokens && original_tokens[index[2]])
		node->skip_expansion[index[1]] = has_single_quotes(
				original_tokens[index[2]]);
	index[1]++;
	index[2]++;
	index[0]++;
}

void	process_regular_token(t_command *node, char **tokens,
		char **original_tokens, int *index)
{
	node->args[index[1]] = ft_strdup(tokens[index[0]]);
	if (original_tokens && original_tokens[index[2]])
		node->skip_expansion[index[1]] = has_single_quotes(
				original_tokens[index[2]]);
	index[1]++;
	index[2]++;
	index[0]++;
}

int	is_redirection(char *token)
{
	return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

int	is_token_quoted(char **original_tokens, int index)
{
	int	len;

	if (!original_tokens || !original_tokens[index])
		return (0);
	len = ft_strlen(original_tokens[index]);
	if (len >= 2 && ((original_tokens[index][0] == '"'
			&& original_tokens[index][len - 1] == '"')
		|| (original_tokens[index][0] == '\''
			&& original_tokens[index][len - 1] == '\'')))
		return (1);
	return (0);
}

int	ft_arg_count(char **tokens, char **original_tokens, t_command *command)
{
	int	i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]) && !is_token_quoted(original_tokens, i))
		{
			if (!validate_redirection(tokens, i, command))
				return (-1);
			i += 2;
		}
		else
		{
			arg_count++;
			i++;
		}
	}
	return (arg_count);
}
