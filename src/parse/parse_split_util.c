/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/04 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static const char	*skip_delimiters(const char *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}

static const char	*skip_word_with_quotes(const char *s, char c)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (*s && (*s != c || in_single || in_double))
	{
		if (*s == '\'' && !in_double)
			in_single = !in_single;
		else if (*s == '"' && !in_single)
			in_double = !in_double;
		s++;
	}
	return (s);
}

int	count_words(const char *s, char c)
{
	int	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		s = skip_delimiters(s, c);
		if (*s)
		{
			count++;
			s = skip_word_with_quotes(s, c);
		}
	}
	return (count);
}

char	**free_split_array(char **arr)
{
	int	i;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (NULL);
}

void	remove_quotes_from_word(const char *s, char c, char *result)
{
	int	j;
	int	k;
	int	in_single;
	int	in_double;

	j = 0;
	k = 0;
	in_single = 0;
	in_double = 0;
	while (s[j] && (s[j] != c || in_single || in_double))
	{
		if (s[j] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[j] == '"' && !in_single)
			in_double = !in_double;
		else
			result[k++] = s[j];
		j++;
	}
	result[k] = '\0';
}
