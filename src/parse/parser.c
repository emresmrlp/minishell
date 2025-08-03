/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:09:37 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:39:39 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_command	*create_node(void);

char	**split_redirects_for_command(char *command)
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

void	cleanup_command_resources(char *joined, char **tokens,
		char **redirect_split_result)
{
	free(joined);
	free_tokens(tokens);
	free_tokens(redirect_split_result);
}

t_command	*process_single_command(char *shell_command, t_envp *env_list)
{
	char		**redirect_split_result;
	char		*joined;
	char		**tokens;
	t_command	*node;

	redirect_split_result = split_redirects_for_command(shell_command);
	joined = space_strjoin_function(redirect_split_result);
	tokens = ft_split(joined, ' ');
	node = create_node();
	if (!node)
		return (NULL);
	node->env_list = env_list;
	if (!parse_argv(node, tokens, joined))
	{
		cleanup_command_resources(joined, tokens, redirect_split_result);
		free(node);
		return (NULL);
	}
	cleanup_command_resources(joined, tokens, redirect_split_result);
	return (node);
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
