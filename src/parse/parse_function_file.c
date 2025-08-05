/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_function_file.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:30:34 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 16:11:40 by makpolat         ###   ########.fr       */
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

static void	handle_heredoc_redirect(t_command *node, char **tokens, int *j)
{
	if (!tokens[*j + 1])
	{
		error_handler(node,
			"minishell: syntax error near unexpected token `newline'\n", 2);
		return ;
	}
	add_to_fd_array(&node->heredoc_fd, tokens[++(*j)]);
}

static void	handle_append_redirect(t_command *node, char **tokens, int *j)
{
	if (!tokens[*j + 1])
	{
		error_handler(node,
			"minishell: syntax error near unexpected token `newline'\n", 2);
		return ;
	}
	add_to_fd_array(&node->append_fd, tokens[++(*j)]);
}

void	handle_redirection(t_command *node, char **tokens, int *j)
{
	if (ft_strcmp(tokens[*j], "<<") == 0)
		handle_heredoc_redirect(node, tokens, j);
	else if (ft_strcmp(tokens[*j], ">>") == 0)
		handle_append_redirect(node, tokens, j);
	else if (ft_strcmp(tokens[*j], "<") == 0)
		handle_input_redirect(node, tokens, j);
	else if (ft_strcmp(tokens[*j], ">") == 0)
		handle_output_redirect(node, tokens, j);
}
