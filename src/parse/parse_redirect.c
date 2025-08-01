/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/01 12:28:56 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Forward declarations
static void	free_tokens(char **tokens);

// Basit array helper - dosya eklemek için
static void add_to_fd_array(char ***fd_array, char *filename)
{
	int count = 0;
	char **new_array;
	int i;
	
	// Mevcut dosya sayısını say
	if (*fd_array)
	{
		while ((*fd_array)[count])
			count++;
	}
	
	// Yeni array oluştur
	new_array = malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return;
	
	// Eski dosyaları kopyala
	i = 0;
	while (i < count)
	{
		new_array[i] = (*fd_array)[i];
		i++;
	}
	
	// Yeni dosyayı ekle
	new_array[count] = ft_strdup(filename);
	new_array[count + 1] = NULL;
	
	// Eski array'i serbest bırak
	if (*fd_array)
		free(*fd_array);
	
	*fd_array = new_array;
}

// Helper fonksiyonlar
static int has_single_quotes(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

static char **split_with_quotes_preserved(char *str, char delimiter)
{
	// Daha güvenli buffer boyutu hesaplama
	int estimated_tokens = 1;
	int i = 0;
	
	// Token sayısını tahmin et
	while (str[i])
	{
		if (str[i] == delimiter)
			estimated_tokens++;
		i++;
	}
	
	// Buffer boyutunu güvenli şekilde ayarla
	int buffer_size = estimated_tokens + 10; // Güvenlik marjı
	if (buffer_size < 20)
		buffer_size = 20;
	if (buffer_size > 1000)
		buffer_size = 1000;
	
	char **result = malloc(sizeof(char *) * buffer_size);
	if (!result)
		return (NULL);
		
	int count = 0;
	int start = 0;
	i = 0;
	int in_single = 0;
	int in_double = 0;
	
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == delimiter && !in_single && !in_double)
		{
			if (i > start)
			{
				// Buffer overflow kontrolü
				if (count >= buffer_size - 1)
				{
					printf("WARNING: Token buffer full, stopping at %d tokens\n", count);
					break;
				}
				result[count] = ft_substr(str, start, i - start);
				count++;
			}
			// Skip multiple spaces
			while (str[i] == delimiter)
				i++;
			start = i;
			continue;
		}
		i++;
	}
	if (i > start && count < buffer_size - 1)
	{
		result[count] = ft_substr(str, start, i - start);
		count++;
	}
	result[count] = NULL;
	return result;
}

static t_command *create_node(void)
{
	t_command *node;
	
	node = (t_command *)malloc(sizeof(t_command));
	if (!node)
		return NULL;
	node->args = NULL;
	node->input_fds = NULL;
	node->output_fds = NULL;
	node->append_fds = NULL;
	node->heredoc_fds = NULL;
	node->env_list = NULL;
	node->next = NULL;
	node->dollar = 0;
	node->skip_expansion = NULL;
	return node;
}

static void	handle_redirection(t_command *node, char **tokens, int *j)
{
	if (ft_strcmp(tokens[*j], "<<") == 0)
	{
		// Heredoc'u listeye ekle
		add_to_fd_array(&node->heredoc_fds, tokens[++(*j)]);
	}
	else if (ft_strcmp(tokens[*j], ">>") == 0)
	{
		// Append dosyasını listeye ekle
		add_to_fd_array(&node->append_fds, tokens[++(*j)]);
	}
	else if (ft_strcmp(tokens[*j], "<") == 0)
	{
		// Input dosyasını listeye ekle
		add_to_fd_array(&node->input_fds, tokens[++(*j)]);
	}
	else if (ft_strcmp(tokens[*j], ">") == 0)
	{
		// Output dosyasını listeye ekle
		add_to_fd_array(&node->output_fds, tokens[++(*j)]);
	}
}

static int	is_redirection(char *token)
{
	return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0 ||
			ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

static int	validate_redirection(char **tokens, int index)
{
	if (!tokens[index + 1])
	{
		// Allow incomplete redirection - just skip it
		return (1);
	}
	if (is_redirection(tokens[index + 1]))
	{
		error_handler("minishell: syntax error near unexpected token\n");
		return (0);
	}
	
	return (1);
}

static int	ft_arg_count(char **tokens)
{
	int i;
	int arg_count;

	i = 0;
	arg_count = 0;
	while (tokens[i])
	{        
		if (is_redirection(tokens[i]))
		{
			if (!validate_redirection(tokens, i))
				return (-1); // Hata durumu
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

static int	parse_argv(t_command *node, char **tokens, char *original_string)
{
	int i = 0;
	int arg_count;
	
	arg_count = ft_arg_count(tokens);
	if (arg_count == -1) // Hata durumu
		return (0);
	if (arg_count == 0)
		return (1);
	
	node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
	node->skip_expansion = (int *)ft_calloc(arg_count + 1, sizeof(int));
	if (!node->args || !node->skip_expansion)
		return (0);
	
	// Orijinal string'de single quote kontrolü yap
	char **original_tokens = split_with_quotes_preserved(original_string, ' ');
	
	i = 0;
	arg_count = 0;
	int original_index = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]))
		{
			handle_redirection(node, tokens, &i);
			original_index += 2; // Skip redirection operator and filename in original_tokens
			i++; // handle_redirection increments i to point to filename, we need to skip it
		}
		else
		{
			node->args[arg_count] = ft_strdup(tokens[i]);
			
			// Eğer orijinal token single quote içindeyse expansion skip et
			if (original_tokens && original_tokens[original_index])
			{
				node->skip_expansion[arg_count] = has_single_quotes(original_tokens[original_index]);
			}
			arg_count++;
			original_index++;
			i++;
		}
	}
	
	// Original tokens'ı temizle
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
	int		i = 0;
	char	*result = NULL;
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
