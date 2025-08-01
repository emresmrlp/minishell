/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:29:50 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/01 21:22:14 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_parts(char *str, int j, char **result, int *k);

static int	is_in_quotes(char *str, int pos)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (i < pos)
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

static int	count_redirect_tokens(char *token, int *j, int *found_redirect)
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

static int	token_count(char **shell)
{
	int	i;
	int	j;
	int	count;
	int	found_redirect;

	i = 0;
	count = 0;
	while (shell[i])
	{
		j = 0;
		found_redirect = 0;
		while (shell[i][j])
		{
			if ((shell[i][j] == '>' || shell[i][j] == '<') 
				&& !is_in_quotes(shell[i], j))
			{
				count += count_redirect_tokens(shell[i], &j, &found_redirect);
				continue ;
			}
			j++;
		}
		if (!found_redirect)
			count++;
		i++;
	}
	return (count);
}

static int	find_next_redirect(char *str, int start)
{
	int	next_redirect;

	next_redirect = start;
	while (str[next_redirect] && ((str[next_redirect] != '>' 
			&& str[next_redirect] != '<') || is_in_quotes(str, next_redirect)))
		next_redirect++;
	return (next_redirect);
}

static void	process_remaining_redirections(char *str, int next_redirect, 
		char **result, int *k)
{
	char	*remaining_str;

	remaining_str = ft_strdup(str + next_redirect);
	add_parts(remaining_str, 0, result, k);
	free(remaining_str);
}

static void	add_parts(char *str, int j, char **result, int *k)
{
	int			is_double;
	int			remaining_start;
	int			next_redirect;
	static int	depth = 0;

	depth++;
	if (depth > 10)
	{
		depth--;
		return ;
	}
	if (j > 0)
		result[(*k)++] = ft_substr(str, 0, j);
	is_double = (str[j + 1] == str[j]);
	result[(*k)++] = ft_substr(str, j, 1 + is_double);
	j += 1 + is_double;
	remaining_start = j;
	next_redirect = find_next_redirect(str, j);
	if (str[next_redirect])
	{
		if (next_redirect > remaining_start)
			result[(*k)++] = ft_substr(str, remaining_start, 
					next_redirect - remaining_start);
		process_remaining_redirections(str, next_redirect, result, k);
	}
	else
	{
		if (str[j])
			result[(*k)++] = ft_strdup(str + j);
	}
	depth--;
}

static int	find_redirection_pos(char *token)
{
	int	j;

	j = -1;
	while (token[++j] && ((token[j] != '>' && token[j] != '<')
			|| is_in_quotes(token, j)))
		;
	return (j);
}

char	**redirect_split(char **shell)
{
	char	**result;
	int		i;
	int		j;
	int		k;

	if (!shell)
		return (NULL);
	result = malloc(sizeof(char *) * (token_count(shell) + 20));
	if (!result)
		return (NULL);
	i = -1;
	k = 0;
	while (shell[++i])
	{
		j = find_redirection_pos(shell[i]);
		if (shell[i][j])
			add_parts(shell[i], j, result, &k);
		else
			result[k++] = ft_strdup(shell[i]);
	}
	result[k] = NULL;
	return (result);
}
