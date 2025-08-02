/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 21:41:36 by ysumeral         ###   ########.fr       */
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

static int	check_pipe_at_edges(const char *line)
{
	int	i;
	int	len;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
		return (0);
	len = ft_strlen(line) - 1;
	while (len >= 0 && line[len] == ' ')
		len--;
	if (len >= 0 && line[len] == '|')
		return (0);
	return (1);
}

static int	check_double_pipes(const char *line)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
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

int	validate_pipes(const char *line)
{
	if (!check_pipe_at_edges(line))
		return (0);
	return (check_double_pipes(line));
}

static void	handle_quotes_in_pipe(const char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

static void	add_pipe_segment(const char *line, char **shell, int *vars)
{
	char	*temp;

	temp = ft_substr(line, vars[1], vars[2] - vars[1]);
	shell[vars[0]++] = ft_strtrim(temp, " ");
	free(temp);
	vars[1] = vars[2] + 1;
}

static char	**pipe_split(const char *line, int start, int k, int i)
{
	char	**shell;
	char	*temp;
	int		line_len;
	int		vars[5];

	if (!validate_pipes(line))
		return (NULL);
	line_len = ft_strlen(line);
	shell = malloc(sizeof(char *) * (pipe_count(line) + 2));
	if (!shell)
		return (NULL);
	vars[0] = i;
	vars[1] = start;
	vars[2] = k;
	vars[3] = 0;
	vars[4] = 0;
	while (vars[2] < line_len && line[vars[2]])
	{
		handle_quotes_in_pipe(line[vars[2]], &vars[3], &vars[4]);
		if (line[vars[2]] == '|' && !vars[3] && !vars[4])
			add_pipe_segment(line, shell, vars);
		vars[2]++;
	}
	temp = ft_substr(line, vars[1], vars[2] - vars[1]);
	shell[vars[0]++] = ft_strtrim(temp, " ");
	free(temp);
	shell[vars[0]] = NULL;
	return (shell);
}

static void	free_shell_array(char **shell)
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

void	parse_command(char *command_line, t_envp *env_list)
{
	char		**shell;
	t_command	*head;

	shell = pipe_split(command_line, 0, 0, 0);
	if (!shell)
	{
		write(2, "minishell: syntax error near unexpected token\n", 47);
		return ;
	}
	head = add_node(shell, env_list);
	free_shell_array(shell);
	if (head)
	{
		cleanup_exit_status_str(head);
		execute(head);
		memory_free(head);
	}
}
