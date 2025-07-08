/* ************************************************************************** */
/*                           void parse_command(char *command_line, t_envp *env_list)
{
    char **shell = pipe_split(command_line, 0, 0, 0);
    
    if (!shell)
    {
        printf("syntax error: invalid pipe usage\n");
        return;
    }
    add_node(shell, env_list);
    free(shell);
}                                     */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/16 16:16:16 by makpolat         ###   ########.fr       */
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
    int len;
    
    while (line[i] == ' ') i++;
    if (line[i] == '|') return (0);
    

    len = ft_strlen(line) - 1;
    while (len >= 0 && line[len] == ' ') len--;
    if (len >= 0 && line[len] == '|') return 0;
    
    if (ft_strnstr(line, "||", ft_strlen(line))) return (0);
    if (ft_strnstr(line, "| |", ft_strlen(line))) return (0);

    
    return 1;
}

static void pass_quote(const char *line, int *i)
{
    char quote_char;
    
    if (line[*i] == '"' || line[*i] == '\'')
    {
        quote_char = line[*i];
        (*i)++;

        while (line[*i] && line[*i] != quote_char)
            (*i)++;

        if (line[*i] == quote_char)
            (*i)++;
    }
}


static char **pipe_split(const char *line, int start, int k, int i)
{
    char **shell;
    char *temp;

    if (!validate_pipes(line))
        return (NULL);
    shell = malloc(sizeof(char *) * (pipe_count(line) + 2));
    if (!shell)
        return(NULL);
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
    return (shell);
}


void parse_command(char *command_line, t_envp *env_list)
{
    char **shell = pipe_split(command_line, 0, 0, 0);
    
    if (!shell)
    {
        printf("syntax error: invalid pipe usage\n");
        return;
    }
    add_node(shell, env_list);
    free(shell);
}
