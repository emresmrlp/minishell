/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:29:50 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/19 15:00:06 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	token_count(char **shell)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (shell[i])
	{
		j = 0;
		while (shell[i][j])
		{
			if ((shell[i][j] == '>' || shell[i][j] == '<') 
				&& !is_in_quotes(shell[i], j))
			{
				if (j > 0)
					count++;
				count++;
				if (shell[i][j + 1] && shell[i][j + 1] == shell[i][j])
					j++;
				if (shell[i][j + 1])
					count++;
				break ;
			}
			j++;
		}
		if (shell[i][j] == '\0')
			count++;
		i++;
	}
	return (count);
}

static void	add_parts(char *str, int j, char **result, int *k)
{
	int	is_double;

	if (j > 0)
		result[(*k)++] = ft_substr(str, 0, j);
	is_double = (str[j + 1] == str[j]);
	result[(*k)++] = ft_substr(str, j, 1 + is_double);
	j += 1 + is_double;
	if (str[j])
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
	result = malloc(sizeof(char *) * (token_count(shell) + 1));
	if (!result)
		return (NULL);
	i = -1;
	k = 0;
	while (shell[++i])
	{
		j = -1;
		while (shell[i][++j] && ((shell[i][j] != '>' && shell[i][j] != '<')
			|| is_in_quotes(shell[i], j)))
			;
		if (shell[i][j])
			add_parts(shell[i], j, result, &k);
		else
			result[k++] = ft_strdup(shell[i]);
	}
	result[k] = NULL;
	return (result);
}
