/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:52:16 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 21:08:15 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	initialize_parse_argv(t_command *node, char **tokens,
		char *original_string, char ***original_tokens)
{
	int	arg_count;

	*original_tokens = split_with_quotes_preserved(original_string, ' ');
	arg_count = ft_arg_count(tokens, *original_tokens, node);
	if (arg_count == -1)
	{
		if (*original_tokens)
			free_tokens(*original_tokens);
		return (-1);
	}
	if (arg_count == 0)
	{
		if (*original_tokens)
			free_tokens(*original_tokens);
		return (0);
	}
	node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
	node->skip_expansion = (int *)ft_calloc(arg_count + 1, sizeof(int));
	if (!node->args || !node->skip_expansion)
	{
		if (*original_tokens)
			free_tokens(*original_tokens);
		return (-1);
	}
	return (arg_count);
}

static void	process_tokens_loop(t_command *node, char **tokens,
		char **original_tokens, int *index)
{
	while (tokens[index[0]])
	{
		if (is_redirection(tokens[index[0]])
			&& !is_token_quoted(original_tokens, index[0]))
			process_redirection_token(node, tokens, original_tokens, index);
		else
			process_regular_token(node, tokens, original_tokens, index);
	}
}

static int	handle_zero_args(t_command *node, char **tokens)
{
	int	index;

	node->argc = 0;
	index = 0;
	while (tokens[index])
	{
		if (is_redirection(tokens[index]))
			handle_redirection(node, tokens, &index);
		else
			index++;
	}
	return (1);
}

int	parse_argv(t_command *node, char **tokens, char *original_string)
{
	char	**original_tokens;
	int		index[3];
	int		arg_count;

	arg_count = initialize_parse_argv(node, tokens, original_string,
			&original_tokens);
	if (arg_count == -1)
		return (0);
	if (arg_count == 0)
		return (handle_zero_args(node, tokens));
	node->argc = arg_count;
	index[0] = 0;
	index[1] = 0;
	index[2] = 0;
	process_tokens_loop(node, tokens, original_tokens, index);
	if (original_tokens)
		free_tokens(original_tokens);
	return (1);
}
