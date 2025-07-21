/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:02:17 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/21 13:20:43 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	banner(void)
{
	int i;

	i = 2000000000;
	write(1, "\033[H\033[J", 6);
	printf("\033[1;32m");
	printf("\n\n\n███╗   ███╗██╗███╗   ██╗██╗");
	printf("███████╗██╗  ██╗███████╗██╗     ██╗\n");
	printf("████╗ ████║██║████╗  ██║██║██╔════╝");
	printf("██║  ██║██╔════╝██║     ██║\n");
	printf("██╔████╔██║██║██╔██╗ ██║██║███████╗");
	printf("███████║█████╗  ██║     ██║\n");
	printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║");
	printf("██╔══██║██╔══╝  ██║     ██║\n");
	printf("██║ ╚═╝ ██║██║██║ ╚████║██║███████║");
	printf("██║  ██║███████╗███████╗███████╗\n");
	printf("╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝");
	printf("╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
	printf("\nSeriously The Best MiniShell Ever :) (by ysumeral & makpolat)\n");
	printf("\033[0m");
	while (i--)
		(void)0;
	write(1, "\033[H\033[J", 6);
}

void	shell_loop(t_envp *env_list)
{
	char	*command_line;

	banner();
	while (1)
	{
		command_line = readline("\033[38;2;139;170;255mMiniShell$ \033[0m");
		if (!command_line)
		{
			// Ctrl+D (EOF) durumu - programı temiz şekilde kapat
			printf("exit\n");
			break;
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
		t_envp *temp = env_list;
		env_list = env_list->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
