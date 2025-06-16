/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/16 15:11:45 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

static t_command *create_node(void)
{   
    t_command *node = (t_command *)malloc(sizeof(t_command));
    if (!node) return NULL;
    
    node->append_output_fd = NULL;
    node->args = NULL;
    node->heredoc_fd = NULL;
    node->input_fd = NULL;
    node->output_fd = NULL;
    node->next = NULL;
    
    return node;
}

// void print_command_list(t_command *cmd) // Bu Fonksiyon gelen nodeları DOğru bir şekilde ekliyo mu test etmek için silmeyi unutma !!!!
// {
//     int i = 0;

//     while (cmd)
//     {
//         printf("Komut %d:\n", i);
        
//         if (cmd->args)
//         {
//             printf("  Argümanlar:\n");
//             for (int j = 0; cmd->args[j]; j++)
//                 printf("    - %s\n", cmd->args[j]);
//         }

//         if (cmd->input_fd)
//             printf("  Girdi yönlendirme (<): %s\n", cmd->input_fd);
//         if (cmd->output_fd)
//             printf("  Çıktı yönlendirme (>): %s\n", cmd->output_fd);
//         if (cmd->append_output_fd)
//             printf("  Ekli çıktı yönlendirme (>>): %s\n", cmd->append_output_fd);
//         if (cmd->heredoc_fd)
//             printf("  Heredoc (<<): %s\n", cmd->heredoc_fd);

//         cmd = cmd->next;
//         i++;
//     }
// }


static void handle_redirection(t_command *node, char **tokens, int *j)
{
    if (ft_strcmp(tokens[*j], "<<") == 0)
        node->heredoc_fd = ft_strdup(tokens[++(*j)]);

    else if (ft_strcmp(tokens[*j], ">>") == 0)
        node->append_output_fd = ft_strdup(tokens[++(*j)]);

    else if (ft_strcmp(tokens[*j], "<") == 0)
        node->input_fd = ft_strdup(tokens[++(*j)]);

    else if (ft_strcmp(tokens[*j], ">") == 0)
        node->output_fd = ft_strdup(tokens[++(*j)]);
}

static int is_redirection(char *token)
{
    return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0 ||
            ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

static void parse_tokens(t_command *node, char **tokens)
{
    int i = 0;
    int arg_count = 0;
    
    while (tokens[i])
    {
        if (is_redirection(tokens[i]))
            i += 2;
        else
        {
            arg_count++;
            i++;
        }
    }
    if (arg_count > 0)
    {
        node->args = (char **)ft_calloc(arg_count + 1, sizeof(char *));
        i = 0;
        arg_count = 0;
        while (tokens[i])
        {
            if (is_redirection(tokens[i]))
                handle_redirection(node, tokens, &i);
            else
                node->args[arg_count++] = ft_strdup(tokens[i]);
            i++;
        }
    }
}

void add_node(char **shell)
{
    t_command *head = NULL;
    t_command *curr = NULL;
    t_command *node = NULL;
    char **tokens;
    int i = 0;

    while (shell[i])
    {
        node = create_node();
        tokens = ft_split(shell[i], ' ', 0, 0);
        
        parse_tokens(node, tokens);        
        if (!head)
            head = node;
        else
            curr->next = node;
        curr = node;
        i++;
    }
    print_command_list(head);
}
