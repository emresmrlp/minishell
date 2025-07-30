/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:29:50 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/30 17:21:44 by makpolat         ###   ########.fr       */
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
		int found_redirect = 0;
		while (shell[i][j])
		{
			if ((shell[i][j] == '>' || shell[i][j] == '<') 
				&& !is_in_quotes(shell[i], j))
			{
				if (j > 0 && !found_redirect)
					count++; // Part before first redirection
				count++; // Redirection operator
				if (shell[i][j + 1] && shell[i][j + 1] == shell[i][j])
					j++; // Skip double operator
				j++; // Move past operator
				
				// Count tokens after this redirection
				int start = j;
				while (shell[i][j] && shell[i][j] != '>' && shell[i][j] != '<')
					j++;
				if (j > start)
					count++; // Token after redirection
				
				found_redirect = 1;
				continue;
			}
			j++;
		}
		if (!found_redirect)
			count++; // No redirection found, count the whole token
		i++;
	}
	return (count);
}

static void	add_parts(char *str, int j, char **result, int *k)
{
	int	is_double;
	int	remaining_start;
	int	next_redirect;
	char	*remaining_str;
	static int depth = 0;
	
	depth++;
	if (depth > 10)  // Prevent infinite recursion
	{
		depth--;
		return;
	}

	if (j > 0)
		result[(*k)++] = ft_substr(str, 0, j);
	is_double = (str[j + 1] == str[j]);
	result[(*k)++] = ft_substr(str, j, 1 + is_double);
	j += 1 + is_double;
	remaining_start = j;
	
	// Look for next redirection in the remaining part
	next_redirect = j;
	while (str[next_redirect] && ((str[next_redirect] != '>' && str[next_redirect] != '<')
		|| is_in_quotes(str, next_redirect)))
		next_redirect++;
	
	if (str[next_redirect])  // Found another redirection
	{
		// Add the part before the next redirection
		if (next_redirect > remaining_start)
			result[(*k)++] = ft_substr(str, remaining_start, next_redirect - remaining_start);
		// Create a new string starting from the next redirection
		remaining_str = ft_strdup(str + next_redirect);
		// Recursively handle the remaining string starting from position 0
		add_parts(remaining_str, 0, result, k);
		free(remaining_str);
	}
	else  // No more redirections
	{
		if (str[j])
			result[(*k)++] = ft_strdup(str + j);
	}
	depth--;
}

char	**redirect_split(char **shell)
{
	char	**result;
	int		i;
	int		j;
	int		k;

	if (!shell)
		return (NULL);
	result = malloc(sizeof(char *) * (token_count(shell) + 20)); // Extra space for multiple redirections
	if (!result)
		return (NULL);
	i = -1;
	k = 0;
	while (shell[++i])
	{
		j = -1;
		// Check if this shell token has redirection operators (not in quotes)
		while (shell[i][++j] && ((shell[i][j] != '>' && shell[i][j] != '<')
			|| is_in_quotes(shell[i], j)))
			;
		if (shell[i][j])  // Found redirection in this token
			add_parts(shell[i], j, result, &k);
		else
			result[k++] = ft_strdup(shell[i]);
	}
	result[k] = NULL;
	return (result);
}
