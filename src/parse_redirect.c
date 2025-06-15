/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/15 16:48:19 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void print_command_list(t_command *cmd)
{
    int i = 0;

    while (cmd)
    {
        printf("Komut %d:\n", i);
        
        if (cmd->args)
        {
            printf("  Argümanlar:\n");
            for (int j = 0; cmd->args[j]; j++)
                printf("    - %s\n", cmd->args[j]);
        }

        if (cmd->input_fd)
            printf("  Girdi yönlendirme (<): %s\n", cmd->input_fd);
        if (cmd->output_fd)
            printf("  Çıktı yönlendirme (>): %s\n", cmd->output_fd);
        if (cmd->append_output_fd)
            printf("  Ekli çıktı yönlendirme (>>): %s\n", cmd->append_output_fd);
        if (cmd->heredoc_fd)
            printf("  Heredoc (<<): %s\n", cmd->heredoc_fd);

        cmd = cmd->next;
        i++;
    }
}

void add_node(char **shell)
{
    t_command *head = NULL;
    t_command *curr = NULL;
    char **tokens;
    int i = 0;

    while (shell[i])
    {
        t_command *node = create_node();
        tokens = ft_split(shell[i], ' ', 0, 0); // whitespace'e göre böl
        int j = 0;
        while (tokens[j])
        {
            if (strcmp(tokens[j], "<") == 0)
                node->input_fd = ft_strdup(tokens[++j]);        //TODO //FIXME Bu dosyada gelen pipe a göre ayrılmış parçaların içinden redirectleri bulup node a ekleme yapılacak 
                                                                // hatalar var onlar çözülecek
            else if (strcmp(tokens[j], ">") == 0)
                node->output_fd = ft_strdup(tokens[++j]);
            else if (strcmp(tokens[j], ">>") == 0)
                node->append_output_fd = ft_strdup(tokens[++j]);
            else if (strcmp(tokens[j], "<<") == 0)
                node->heredoc_fd = ft_strdup(tokens[++j]);
            else
                node->args = tokens; // yönlendirme değilse argümana ekle
            j++;
        }
        if (!head)
            head = node;
        else
            curr->next = node;
        curr = node;
        free(tokens);
        i++;
    }
    print_command_list(head); // debug için
}



