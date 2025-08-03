/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:29:50 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 10:53:55 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	redirection_loop(char *str, int redirect, char **res, int *k);
static void	add_redirect_parts(char *str, int j, char **result, int *k);

static int	token_count(char **shell)
{
	int	i;
	int	j;
	int	count;
	int	find_redirect;

	i = 0;
	count = 0;
	while (shell[i])
	{
		j = 0;
		find_redirect = 0;
		while (shell[i][j])
		{
			if (process_token_char(shell[i], &j, &find_redirect, &count))
				continue ;
		}
		if (!find_redirect)
			count++;
		i++;
	}
	return (count);
}

static void	redirection_loop(char *str, int redirect, char **res, int *k)
{
	char	*remaining_str;

	remaining_str = ft_strdup(str + redirect);
	add_redirect_parts(remaining_str, 0, res, k);
	free(remaining_str);
}

static void	add_redirect_parts(char *str, int j, char **result, int *k)
{
	int	is_double;
	int	remaining_start;
	int	next_redirect;

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
		redirection_loop(str, next_redirect, result, k);
	}
	else if (str[j])
		result[(*k)++] = ft_strdup(str + j);
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
			add_redirect_parts(shell[i], j, result, &k);
		else
			result[k++] = ft_strdup(shell[i]);
	}
	result[k] = NULL;
	return (result);
}
