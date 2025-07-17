/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:02:17 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/17 15:22:29 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	banner(void)
{
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
	usleep(1500000);
	write(1, "\033[H\033[J", 6);
}

void	read_input(t_envp *env_list)
{
	char	*command_line;

	banner();
	while (1)
	{
		command_line = readline("MiniShell$ ");
		if (command_line && *command_line)
		{
			add_history(command_line);
			parse_command(command_line, env_list);
		}
		free(command_line);
	}
}
