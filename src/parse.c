/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/14 19:25:09 by makpolat         ###   ########.fr       */
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



// static void delete_quote(char **tokens) // tırnakları siliyor örn: echo "Emre Akpolat" = (echo), (Emre Akpolat) (iki parçaya ayırıp tırnakları attı)
// {
// 	int i = 0;
// 	int len;
// 	char quote;

// 	while (tokens[i])
// 	{
// 		len = ft_strlen(tokens[i]);
// 		if (len >= 2)
// 		{
// 			quote = tokens[i][0];
// 			if ((quote == '\'' || quote == '"') && tokens[i][len - 1] == quote)
// 			{
// 				char *tmp = tokens[i];
// 				tokens[i] = ft_substr(tokens[i], 1, len - 2);
// 				free(tmp);
// 			}
// 		}
// 		i++;
// 	}
// }


static int pipe_count(const char *line)
{
    int i = 0;
    int count = 0;
    
    while (line[i])
    {
        if (line[i] == '|')
            count++;
        i++;
    }
    return (count);
}

int validate_pipes(const char *line)
{
    int i = 0;
    
    // Başında pipe varsa hata
    while (line[i] == ' ') i++;
    if (line[i] == '|') printf("hata\n");
    
    // Sonunda pipe varsa hata  
    int len = ft_strlen(line) - 1;
    while (len >= 0 && line[len] == ' ') len--; //////// TODO //FIXME eror vars ayırmasın commandlere boşa yer kaplıyo
    if (len >= 0 && line[len] == '|') return 0;
    
    // Yan yana pipe varsa hata
    if (strstr(line, "||")) return 0;
    
    return 1; // Geçerli
}
static void pass_quote(const char *line, int *i)
{
    if (line[*i] == '"')
    {
        (*i)++; // Açılış tırnağından sonra başla
        while (line[*i] && line[*i] != '"')
            (*i)++;
        if (line[*i] == '"') // Kapanış tırnağını da geç
            (*i)++;
    }
}

char **pipe_split(const char *line)
{
    char **shell = malloc(sizeof(char *) * (pipe_count(line) + 2));
    int i = 0, start = 0, k = 0;
    char *temp;

    
    if (!validate_pipes(line))
        printf("minishell: syntax error near unexpected token `|'\n");
    if (!shell)
        printf("Error: Memory allocation failed\n");
    while (line[k])
    {
        pass_quote(line, &k);
        if (line[k] == '|')
        {
            temp = ft_substr(line, start, k - start);
            shell[i++] = ft_strtrim(temp, " ");
            free(temp);
            start = k + 1;
        }
        k++;
    }
    temp = ft_substr(line, start, k - start);
    shell[i++] = ft_strtrim(temp, " ");
    free(temp);
    shell[i] = NULL;
    return shell;
}

void parse_command(char *command_line)
{
    char **shell = pipe_split(command_line);
    
    if (!shell)
        return; // Hata durumunda çık
        
    int i = 0;
    while (shell[i])
    {
        printf("Command %d '%s'\n", i,shell[i]);
        i++;
    }
}
