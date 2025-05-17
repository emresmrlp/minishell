/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/05/17 18:23:32 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../Libft/libft.h"

int	main(void)
{
	char	*command_line;

	while (1)
	{
		command_line = readline("MiniShell: ");
		if (command_line && *command_line)
		{
			add_history(command_line);
			if (!ft_strncmp(command_line, "exit",ft_strlen(command_line)))
				break ;
			parse_command(command_line);
			printf("Girilen komut: %s\n", command_line);
		}
	}
	return (0);
}
