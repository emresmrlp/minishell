/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:46:11 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/10 14:10:07 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countwords(const char *s, char c)
{
	int	i;
	int	count;
	int	in_quote;

	i = 0;
	if (!*s)
		return (0);
	count = 0;
	in_quote = 0;
	while (*s)
	{
		while (*s == c && !in_quote)
			s++;
		if (*s)
		{
			count++;
			while (*s && (*s != c || in_quote))
			{
				if (*s == '\'' || *s == '"')
					in_quote = !in_quote;
				s++;
			}
		}
	}
	return (count);
}

static char	**check(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (0);
}

static int	get_len(const char *s, char c)
{
	int	j;
	int	in_quote;

	j = 0;
	in_quote = 0;
	while (s[j] && (s[j] != c || in_quote))
	{
		if (s[j] == '\'' || s[j] == '"')
			in_quote = !in_quote;
		j++;
	}
	return (j);
}

char	**ft_split(char const *s, char c, int i, int j)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = (char **)malloc((countwords(s, c) + 1) * sizeof(char *));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			j = get_len(s, c);
			tab[i] = ft_substr(s, 0, j);
			if (tab[i++] == NULL)
				return (check(tab));
			s += j;
		}
	}
	tab[i] = NULL;
	return (tab);
}
