/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/07 14:57:11 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../Libft/libft.h"

void error(char *message)
{
	write(2, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}

void voidke(char **argv, char **envp)
{
	(void)argv;
	(void)envp;	
}

void read_input()
{
	char *command_line;

	while (1)
	{
		command_line = readline("MiniShell: ");
		if (command_line && *command_line)
		{
			add_history(command_line);
			parse_command(command_line);
			if (!ft_strncmp(command_line, "exit", ft_strlen(command_line)))
			{
				free(command_line);
				clear_history();
				break ;
			}
		}
		free(command_line);
	}
}


int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	if ((ft_strcmp(*argv,"./minishell")) || (argc != 1))
		error("Wrong program name or number just run 'minishell'\n");
	voidke(argv, envp); // sussun diye konuldu

	read_input();

	

	return (0);
}