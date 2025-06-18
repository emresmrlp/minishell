/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/06/18 15:05:13 by makpolat         ###   ########.fr       */
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




typedef struct s_command {
    char            **args;     
    char             *input_fd;   // >
    char             *output_fd; // <
    char             *append_output_fd; // >>
    char             *heredoc_fd; // <<
    int               dollar;
    struct s_command *next;
} t_command;

// t_command **shell;

// shell[0].args[0]=ls;
// shell[0].args[1]=-l;
// shell[0].input_fd=a;
// shell[1].args[0]=grep;
// shell[1].args[1]=m;
// shell[1].outfile_fd=c;
// < a ls -l | >a > b >c grep m

void parse_command(char *command_line);
void add_node(char **shell);
void error(char *message);
void	parse_dollar(t_command *head);
int ft_strcmp(const char *s1, const char *s2);
void print_command_list(t_command *cmd);




#endif
