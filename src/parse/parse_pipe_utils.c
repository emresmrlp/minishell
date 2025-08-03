/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:38:32 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pipe_count(const char *line)
{
	int	i;
	int	count;
	int	in_single;
	int	in_double;

	i = 0;
	count = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (line[i] == '|' && !in_single && !in_double)
			count++;
		i++;
	}
	return (count);
}

void	handle_quotes_in_pipe(const char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

void	add_pipe_segment(const char *line, char **shell, int *vars)
{
	char	*temp;

	temp = ft_substr(line, vars[1], vars[2] - vars[1]);
	shell[vars[0]++] = ft_strtrim(temp, " ");
	free(temp);
	vars[1] = vars[2] + 1;
}

void	free_shell_array(char **shell)
{
	int	i;

	i = 0;
	while (shell[i])
	{
		free(shell[i]);
		i++;
	}
	free(shell);
}

void	finalize_pipe_split(const char *line, char **shell, int *vars)
{
	char	*temp;

	temp = ft_substr(line, vars[1], vars[2] - vars[1]);
	shell[vars[0]++] = ft_strtrim(temp, " ");
	free(temp);
	shell[vars[0]] = NULL;
}
