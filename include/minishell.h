/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/10 13:10:48 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>


typedef struct s_envp {
    char            *key;
    char            *value;
    struct s_envp  *next;
} t_envp;


typedef struct s_command {
    char                **args;     
    char                *input_fd;   // >
    char                *output_fd; // <
    char                *append_output_fd; // >>
    char                *heredoc_fd; // <<
    t_envp              *env_list;    // Environment linked list
    int                 dollar;
    struct s_command    *next;
} t_command;



// t_command **shell;

// shell[0].args[0]=ls;
// shell[0].args[1]=-l;
// shell[0].input_fd=a;
// shell[1].args[0]=grep;
// shell[1].args[1]=m;
// shell[1].outfile_fd=c;
// < a ls -l | >a > b >c grep m

void parse_command(char *command_line, t_envp *env_list);
void add_node(char **shell, t_envp *env_list);
void error(char *message);
void	parse_dollar(t_command *head);
int ft_strcmp(const char *s1, const char *s2);
void    split_built_in(t_command *head);
void print_command_list(t_command *cmd);

// // Environment linked list functions
// t_envp *create_env_list(char **envp);
// char *find_env_value(t_envp *env_list, char *key);
// void add_env_node(t_envp **head, char *key, char *value);
// void update_env_value(t_envp *env_list, char *key, char *value);
// void free_env_list(t_envp *env_list);




#endif
