/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/04 12:38:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "../Libft/libft.h"
#include "../include/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdio.h>

// t_command *create_command_pipe(char **tokens)
// {
//     t_command *first_node = NULL;
//     t_command *current_node = NULL;

// }



static void delete_quote(char **tokens) // tırnakları siliyor örn: echo "Emre Akpolat" = (echo), (Emre Akpolat) (iki parçaya ayırıp tırnakları attı)
{
	int i = 0;
	int len;
	char quote;

	while (tokens[i])
	{
		len = ft_strlen(tokens[i]);
		if (len >= 2)
		{
			quote = tokens[i][0];
			if ((quote == '\'' || quote == '"') && tokens[i][len - 1] == quote)
			{
				char *tmp = tokens[i];
				tokens[i] = ft_substr(tokens[i], 1, len - 2);
				free(tmp);
			}
		}
		i++;
	}
}


void parse_command(char *command_line)
{
	char **tokens;
	int i;
	//t_command *first_node;
	
	tokens = ft_split(command_line, ' ', 0, 0);
	delete_quote(tokens);
	i = 0;
	//first_node = create_command_pipe(tokens);
	while (tokens[i])
	{
		printf(" %s \n",tokens[i]);
		i++;
	}
	
	
}