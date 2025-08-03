/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:13:21 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 16:11:39 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_single_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	calculate_buffer_size(char *str, char delimiter)
{
	int	estimated_tokens;
	int	buffer_size;
	int	i;

	estimated_tokens = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
			estimated_tokens++;
		i++;
	}
	buffer_size = estimated_tokens + 10;
	if (buffer_size < 20)
		buffer_size = 20;
	if (buffer_size > 1000)
		buffer_size = 1000;
	return (buffer_size);
}

int	is_redirection(char *token)
{
	return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

int	is_token_quoted(char **original_tokens, int index)
{
	char	*token;
	int		len;

	if (!original_tokens || index < 0)
		return (0);
	token = original_tokens[index];
	if (!token || !*token)
		return (0);
	len = ft_strlen(token);
	if (len < 2)
		return (0);
	if ((token[0] == '\'' && token[len - 1] == '\'')
		|| (token[0] == '"' && token[len - 1] == '"'))
		return (1);
	return (0);
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
