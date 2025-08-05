/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:39:31 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_command	*create_command_node(char *shell_cmd, t_envp *env_list,
		t_command **head)
{
	t_command	*node;

	node = process_single_command(shell_cmd, env_list);
	if (!node)
	{
		memory_free(*head);
		return (NULL);
	}
	return (node);
}

t_command	*add_node(char **shell, t_envp *env_list)
{
	t_command	*head;
	t_command	*curr;
	t_command	*node;
	int			i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (shell[i])
	{
		node = create_command_node(shell[i], env_list, &head);
		if (!node)
			return (NULL);
		if (!head)
			head = node;
		else
			curr->next = node;
		curr = node;
		i++;
	}
	if (head)
		parse_dollar(head);
	return (head);
}
