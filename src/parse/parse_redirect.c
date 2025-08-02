/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 21:40:35 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Forward declarations
static void	free_tokens(char **tokens);

// Basit array helper - dosya eklemek için
static void	add_to_fd_array(char ***fd_array, char *filename)
{
	int		count;
	char	**new_array;
	int		i;

	count = 0;
	if (*fd_array)
	{
		while ((*fd_array)[count])
			count++;
	}
	new_array = malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return ;
	i = 0;
	while (i < count)
	{
		new_array[i] = (*fd_array)[i];
		i++;
	}
	new_array[count] = ft_strdup(filename);
	new_array[count + 1] = NULL;
	if (*fd_array)
		free(*fd_array);
	*fd_array = new_array;
}

static int	has_single_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

static int	calculate_buffer_size(char *str, char delimiter)
{
	int	estimated_tokens;
	int	buffer_size;
	int	i;

	estimated_tokens = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
			estimated_tokens++;
		i++;
	}
	buffer_size = estimated_tokens + 10;
	if (buffer_size < 20)
		buffer_size = 20;
	if (buffer_size > 1000)
		buffer_size = 1000;
	return (buffer_size);
}

static void	process_token_split(char *str, char delimiter, char **result, 
		int *params)
{
	if (str[params[3]] == '\'' && !params[2])
		params[1] = !params[1];
	else if (str[params[3]] == '"' && !params[1])
		params[2] = !params[2];
	else if (str[params[3]] == delimiter && !params[1] && !params[2])
	{
		if (params[3] > params[4])
		{
			if (params[0] >= params[5] - 1)
				return ;
			result[params[0]] = ft_substr(str, params[4], params[3] - params[4]);
			params[0]++;
		}
		while (str[params[3]] == delimiter)
			params[3]++;
		params[4] = params[3];
		return ;
	}
	params[3]++;
}

static char	**split_with_quotes_preserved(char *str, char delimiter)
{
	char	**result;
	int		params[6];

	params[5] = calculate_buffer_size(str, delimiter);
	result = malloc(sizeof(char *) * params[5]);
	if (!result)
		return (NULL);
	params[0] = 0;
	params[1] = 0;
	params[2] = 0;
	params[3] = 0;
	params[4] = 0;
	while (str[params[3]])
		process_token_split(str, delimiter, result, params);
	if (params[3] > params[4] && params[0] < params[5] - 1)
	{
		result[params[0]] = ft_substr(str, params[4], params[3] - params[4]);
		params[0]++;
	}
	result[params[0]] = NULL;
	return (result);
}

static t_command	*create_node(void)
{
	t_command	*node;

	node = (t_command *)malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->input_fd = NULL;
	node->output_fd = NULL;
	node->append_fd = NULL;
	node->heredoc_fd = NULL;
	node->env_list = NULL;
	node->next = NULL;
	node->dollar = 0;
	node->skip_expansion = NULL;
	node->exit_status = 0;
	return (node);
}

static int	is_token_quoted(char **original_tokens, int index)
{
	int	len;

	if (!original_tokens || !original_tokens[index])
		return (0);
	len = ft_strlen(original_tokens[index]);
	if (len >= 2 && ((original_tokens[index][0] == '"' 
				&& original_tokens[index][len - 1] == '"') 
			|| (original_tokens[index][0] == '\'' 
				&& original_tokens[index][len - 1] == '\'')))
		return (1);
	return (0);
}

static void	handle_redirection(t_command *node, char **tokens, int *j, 
		char **original_tokens, int original_index)
{
	if (is_token_quoted(original_tokens, original_index))
		return ;
	if (ft_strcmp(tokens[*j], "<<") == 0)
		add_to_fd_array(&node->heredoc_fd, tokens[++(*j)]);
	else if (ft_strcmp(tokens[*j], ">>") == 0)
		add_to_fd_array(&node->append_fd, tokens[++(*j)]);
	else if (ft_strcmp(tokens[*j], "<") == 0)
		add_to_fd_array(&node->input_fd, tokens[++(*j)]);
	else if (ft_strcmp(tokens[*j], ">") == 0)
		add_to_fd_array(&node->output_fd, tokens[++(*j)]);
}

static int	is_redirection(char *token)
{
	return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0 ||
			ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

static int	validate_redirection(char **tokens, int index, t_command *command)
{
	if (!tokens[index + 1])
	{
		// Allow incomplete redirection - just skip it
		return (1);
	}
	if (is_redirection(tokens[index + 1]))
	{
		error_handler(command, "minishell: syntax error near unexpected token\n", 2);
		return (0);
	}
	
	return (1);
}

static int	ft_arg_count(char **tokens, char **original_tokens, t_command *command)
{
	int	i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]) && !is_token_quoted(original_tokens, i))
		{
			if (!validate_redirection(tokens, i, command))
				return (-1);
			i += 2;
		}
		else
		{
			arg_count++;
			i++;
		}
	}
	return (arg_count);
}

static void	process_redirection_token(t_command *node, char **tokens, 
		char **original_tokens, int *indexes)
{
	handle_redirection(node, tokens, &indexes[0], original_tokens, indexes[2]);
	if (!is_token_quoted(original_tokens, indexes[2]))
	{
		indexes[2] += 2;
		indexes[0]++;
		return ;
	}
	node->args[indexes[1]] = ft_strdup(tokens[indexes[0]]);
	if (original_tokens && original_tokens[indexes[2]])
		node->skip_expansion[indexes[1]] = has_single_quotes(original_tokens[indexes[2]]);
	indexes[1]++;
	indexes[2]++;
	indexes[0]++;
}

static void	process_regular_token(t_command *node, char **tokens, 
		char **original_tokens, int *indexes)
{
	node->args[indexes[1]] = ft_strdup(tokens[indexes[0]]);
	if (original_tokens && original_tokens[indexes[2]])
		node->skip_expansion[indexes[1]] = has_single_quotes(original_tokens[indexes[2]]);
	indexes[1]++;
	indexes[2]++;
	indexes[0]++;
}

static int	parse_argv(t_command *node, char **tokens, char *original_string)
{
	int		arg_count;
	char	**original_tokens;
	int		indexes[3];

	original_tokens = split_with_quotes_preserved(original_string, ' ');
	arg_count = ft_arg_count(tokens, original_tokens, node);
	if (arg_count == -1)
	{
		if (original_tokens)
			free_tokens(original_tokens);
		return (0);
	}
	if (arg_count == 0)
	{
		if (original_tokens)
			free_tokens(original_tokens);
		return (1);
	}
	node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
	node->skip_expansion = (int *)ft_calloc(arg_count + 1, sizeof(int));
	if (!node->args || !node->skip_expansion)
	{
		if (original_tokens)
			free_tokens(original_tokens);
		return (0);
	}
	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 0;
	while (tokens[indexes[0]])
	{
		if (is_redirection(tokens[indexes[0]]) && !is_token_quoted(original_tokens, indexes[0]))
			process_redirection_token(node, tokens, original_tokens, indexes);
		else
			process_regular_token(node, tokens, original_tokens, indexes);
	}
	if (original_tokens)
		free_tokens(original_tokens);
	return (1);
}

static void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

static char	**split_redirects_for_command(char *command)
{
	char	**temp_shell;
	char	**result;

	temp_shell = malloc(sizeof(char *) * 2);
	if (!temp_shell)
		return (NULL);
	temp_shell[0] = command;
	temp_shell[1] = NULL;
	result = redirect_split(temp_shell);
	free(temp_shell);
	return (result);
}

static char	*space_strjoin_function(char **arr)
{
	int		i;
	char	*result;
	char	*tmp;

	if (!arr || !arr[0])
		return (NULL);
	result = ft_strdup(arr[0]);
	i = 1;
	while (arr[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		tmp = result;
		result = ft_strjoin(result, arr[i]);
		free(tmp);
		i++;
	}
	return (result);
}

t_command	*add_node(char **shell, t_envp *env_list)
{
	t_command	*head = NULL;
	t_command	*curr = NULL;
	t_command	*node;
	char		**redirect_split_result;
	char		*joined;
	char		**tokens;
	int			i;

	i = 0;
	while (shell[i])
	{
		redirect_split_result = split_redirects_for_command(shell[i]);
		joined = space_strjoin_function(redirect_split_result);
		tokens = ft_split(joined, ' ');
		node = create_node();
		if (!node)
		{
			memory_free(head);
			return (NULL);
		}
		node->env_list = env_list;

		if (!parse_argv(node, tokens, joined))
		{
			free(joined);
			free_tokens(tokens);
			free_tokens(redirect_split_result);
			free(node);
			memory_free(head); // Önceki node'ları da temizle
			return (NULL);
		}
		if (!head)
			head = node;
		else
			curr->next = node;
		curr = node;

		free(joined);
		free_tokens(tokens);
		free_tokens(redirect_split_result);
		i++;
	}
	if (head)
		parse_dollar(head);
	return (head);
}
