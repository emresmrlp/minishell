/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:46:11 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/15 07:41:26 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countwords(const char *s, char c, int sq, int dq)
{
	int	count;

	if (!*s)
		return (0);
	count = 0;
	sq = 0;
	dq = 0;
	while (*s)
	{
		while (*s == c && !sq && !dq)
			s++;
		if (*s)
		{
			count++;
			while (*s && (*s != c || sq || dq))
			{
				if (*s == '\'' && !dq)
					sq = !sq;
				else if (*s == '"' && !sq)
					dq = !dq;
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
	int	in_single_quote;
	int	in_double_quote;

	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (s[j] && (s[j] != c || in_single_quote || in_double_quote))
	{
		if (s[j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (s[j] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		j++;
	}
	return (j);
}

static char	*remove_quotes(const char *s)
{
	char	*result;
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;

	result = malloc(ft_strlen(s) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (s[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = s[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	**ft_split(char const *s, char c, int i, int j)
{
	char	**tab;
	char	*temp;

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
			break ;
		j = get_len(s, c);
		temp = ft_substr(s, 0, j);
		if (temp == NULL)
			return (check(tab));
		tab[i] = remove_quotes(temp);
		free(temp);
		if (tab[i++] == NULL)
			return (check(tab));
		s += j;
	}
	tab[i] = NULL;
	return (tab);
}
