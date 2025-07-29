/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:02:17 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 14:51:22 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shell_loop(t_envp *env_list)
{
	char	*command_line;
	t_envp	*temp;

	write(1, "\033[H\033[J", 6);
	while (1)
	{
		command_line = readline("\033[38;2;139;170;255mMiniShell$ \033[0m");
		if (!command_line)
		{
			printf("exit\n");
			break ;
		}
		if (*command_line)
		{
			add_history(command_line);
			parse_command(command_line, env_list);
		}
		free(command_line);
	}
	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
