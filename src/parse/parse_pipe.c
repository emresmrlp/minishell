/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/01 14:43:28 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	pipe_count(const char *line)
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

int	validate_pipes(const char *line)
{
	int	i;
	int	len;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
		return (0);
	len = ft_strlen(line) - 1;
	while (len >= 0 && line[len] == ' ')
		len--;
	if (len >= 0 && line[len] == '|')
		return (0);
	
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (line[i] == '|' && !in_single && !in_double)
		{
			if (line[i + 1] == '|')
				return (0);
			if (line[i + 1] == ' ' && line[i + 2] == '|')
				return (0);
		}
		i++;
	}
	return (1);
}

static char	**pipe_split(const char *line, int start, int k, int i)
{
	char	**shell;
	char	*temp;
	int		line_len;
	int		in_single, in_double;

	if (!validate_pipes(line))
		return (NULL);
	line_len = ft_strlen(line);
	shell = malloc(sizeof(char *) * (pipe_count(line) + 2));
	if (!shell)
		return (NULL);
	in_single = 0;
	in_double = 0;
	while (k < line_len && line[k])
	{
		if (line[k] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[k] == '"' && !in_single)
			in_double = !in_double;
		else if (line[k] == '|' && !in_single && !in_double)
		{
			temp = ft_substr(line, start, k - start);
			shell[i++] = ft_strtrim(temp, " ");
			free(temp);
			start = k + 1;
		}
		k++;
	}
	temp = ft_substr(line, start, k - start);
	shell[i++] = ft_strtrim(temp, " ");
	free(temp);
	shell[i] = NULL;
	return (shell);
}

void	parse_command(char *command_line, t_envp *env_list)
{
	char		**shell;
	int			i;
	t_command	*head;

	shell = pipe_split(command_line, 0, 0, 0);
	if (!shell)
	{
		error_handler("minishell: syntax error near unexpected token\n");
		return ;
	}
	head = add_node(shell, env_list);
	
	i = 0;
	while (shell[i])
	{
		free(shell[i]);
		i++;
	}
	free(shell);
	
	// Execute and cleanup the command structure
	if (head)
	{
		cleanup_exit_status_str();
		execute(head);
		memory_free(head);
	}
}
