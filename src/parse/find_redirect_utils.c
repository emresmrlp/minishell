/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_redirect_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:23:45 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 17:27:09 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	inside_quote(char *str, int pos)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (i < pos)
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	return (single_quote || double_quote);
}

int	token_count_redirect(char *token, int *j, int *found_redirect)
{
	int	start;
	int	token_count;

	token_count = 0;
	if (*j > 0 && !*found_redirect)
		token_count++;
	token_count++;
	if (token[*j + 1] && token[*j + 1] == token[*j])
		(*j)++;
	(*j)++;
	start = *j;
	while (token[*j] && token[*j] != '>' && token[*j] != '<')
		(*j)++;
	if (*j > start)
		token_count++;
	*found_redirect = 1;
	return (token_count);
}

int	process_token_char(char *token, int *j, int *find_redirect, int *count)
{
	if ((token[*j] == '>' || token[*j] == '<') && !inside_quote(token, *j))
	{
		*count += token_count_redirect(token, j, find_redirect);
		return (1);
	}
	(*j)++;
	return (0);
}

int	find_next_redirect(char *str, int start)
{
	int	next_redirect;

	next_redirect = start;
	while (str[next_redirect] && ((str[next_redirect] != '>'
		&& str[next_redirect] != '<') || inside_quote(str, next_redirect)))
		next_redirect++;
	return (next_redirect);
}

int	find_redirection_pos(char *token)
{
	int	j;

	j = -1;
	while (token[++j] && ((token[j] != '>' && token[j] != '<')
			|| inside_quote(token, j)))
		;
	return (j);
}

