/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/10 01:54:06 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_word_length(const char *s, char c)
{
	int	len;
	int	in_single;
	int	in_double;

	len = 0;
	in_single = 0;
	in_double = 0;
	while (s[len] && (s[len] != c || in_single || in_double))
	{
		if (s[len] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[len] == '"' && !in_single)
			in_double = !in_double;
		len++;
	}
	return (len);
}

static char	*create_word(const char *s, char c)
{
	char	*result;
	int		len;

	len = get_word_length(s, c);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	remove_quotes_from_word(s, c, result);
	return (result);
}

static const char	*skip_to_next_word(const char *s, char c)
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

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;

	if (!s)
		return (NULL);
	tab = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			tab[i] = create_word(s, c);
			if (!tab[i])
				return (free_split_array(tab));
			i++;
			s = skip_to_next_word(s, c);
		}
	}
	tab[i] = NULL;
	return (tab);
}
