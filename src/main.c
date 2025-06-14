/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/14 13:49:58 by makpolat         ###   ########.fr       */
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
				break ;
			printf("Girilen komut: %s\n", command_line);
		}
	}
}


int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	if ((ft_strncmp(*argv,"./minishell", 12)) || (argc != 1))
		error("Wrong program name or number just run 'minishell'\n");
	voidke(argv, envp); // sussun diye konuldu

	read_input();

	printf("=== PIPE SPLIT TEST ===\n");
    
    char *tests[] = {
        // Geçerli testler
        "ls -la | grep test | wc -l",
        "cat file.txt | head -10", 
        "ls -la",
        
        // Hatalı testler
        "|||",
        "| grep test",
        "ls -la |",
        "ls || grep",
        "ls | | grep",
        
        NULL
    };
    
    for (int i = 0; tests[i]; i++) {
        printf("\nTest %d: \"%s\"\n", i+1, tests[i]);
        parse_command(tests[i]);
    }

	return (0);
}