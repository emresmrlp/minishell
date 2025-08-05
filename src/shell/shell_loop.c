/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:02:17 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 15:52:08 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shell_loop(t_envp *env_list)
{
	char	*command_line;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
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
	free_env_list(env_list);
}
