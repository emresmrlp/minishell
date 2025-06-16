/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:58:12 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/16 17:20:50 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int ft_strcmp(const char *s1, const char *s2) // dünyanın en basit fonksiyonunun amcası falan
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

static t_command *create_node(void) // yeni node oluşyurup sonra ilerde başımız ağrımasın diye NULL ile başlatıyoruz parametreleri
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

void print_command_list(t_command *cmd) // Bu Fonksiyon gelen nodeları DOğru bir şekilde ekliyo mu test etmek için silmeyi unutma !!!!
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


static void handle_redirection(t_command *node, char **tokens, int *j) // bu fonksiyonda gelen redirectlerden sonraki token yani hedef dosyayı buluyoruz 'ls > out.txt'
{                                                                       //burada bulacağı yer "out.txt" //mustafaya binlerce teşekkür burdan çünkü kafamda böyle bir fonksiyonun canlanmasını sağladı
    if (!tokens[*j + 1])
    {
        printf("minishell: syntax error near unexpected token `newline'\n");
        return;
    }

    if (ft_strcmp(tokens[*j], "<<") == 0)
        node->heredoc_fd = ft_strdup(tokens[++(*j)]);
    else if (ft_strcmp(tokens[*j], ">>") == 0)
        node->append_output_fd = ft_strdup(tokens[++(*j)]);
    else if (ft_strcmp(tokens[*j], "<") == 0)
        node->input_fd = ft_strdup(tokens[++(*j)]);
    else if (ft_strcmp(tokens[*j], ">") == 0)
        node->output_fd = ft_strdup(tokens[++(*j)]);
}

static int is_redirection(char *token) // bu zaten dünyanın en basit fonksiyonu bakınca anlamıyosan köyüne dön
{
    return (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0 ||
            ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0);
}

static int ft_arg_count(char **tokens) // adından da anlaşılacağı üzere
{
    int i;
    int arg_count;

    i = 0;
    arg_count = 0;
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
    return (arg_count);
}

static void parse_argv(t_command *node, char **tokens) // dostum şimdi bu fonksiyon gelen argüman sayısına göre yer ayırıyo ve eğer o argümanların içinde redirect varsa
{                                                      // redirect varsa handle_redirect fonksiyonunda onların adını saklıyor dehşet bişey yani eğer yoksa da argümanlara strdup ile yer ayırıp döndürüyor
    int i = 0;
    int arg_count;
    
    arg_count = ft_arg_count(tokens);
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

void add_node(char **shell) // bu fonksiyon gelen pipe a göre ayrılan blokları nodelara eklemek için
{                               //TODO shell ve token değişkenlerini hala daha free etmedik unutma
    t_command *head = NULL;
    t_command *curr = NULL;
    t_command *node = NULL;
    char **tokens;
    int i;

    i = 0;
    while (shell[i])
    {
        node = create_node();
        tokens = ft_split(shell[i], ' ', 0, 0);
        
        parse_argv(node, tokens);
        if (!head)
            head = node;
        else
            curr->next = node;
        curr = node;
        i++;
    }
    print_command_list(head);
}
