/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:46:11 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/17 15:19:55 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countwords(const char *s, char c, int i, int j)
{
	int	count;

	if (!*s)
		return (0);
	count = 0;
	i = 0;
	j = 0;
	while (*s)
	{
		while (*s == c && !i && !j)
			s++;
		if (*s)
		{
			count++;
			while (*s && (*s != c || i || j))
			{
				if (*s == '\'' && !j)
					i = !i;
				else if (*s == '"' && !i)
					j = !j;
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

static int	get_len_and_remove_quotes(const char *s, char c, char *result)
{
	int	j;
	int	k;
	int	in_single_quote;
	int	in_double_quote;

	j = 0;
	k = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (s[j] && (s[j] != c || in_single_quote || in_double_quote))
	{
		if (s[j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (s[j] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[k++] = s[j];
		j++;
	}
	result[k] = '\0';
	return (j);
}

static char	*create_word(const char *s, char c)
{
	char	*result;
	int		len;

	len = 0;
	while (s[len] && (s[len] != c))
		len++;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	get_len_and_remove_quotes(s, c, result);
	return (result);
}

char	**ft_split(char const *s, char c, int i, int j)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = (char **)malloc((countwords(s, c, 0, 0) + 1) * sizeof(char *));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			tab[i] = create_word(s, c);
			if (tab[i++] == NULL)
				return (check(tab));
			while (*s && *s != c)
				s++;
		}
	}
	tab[i] = NULL;
	return (tab);
}
