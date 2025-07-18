/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 12:49:00 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../external/libft/libft.h"
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

# define FAILURE 1
# define SUCCESS 0

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_command
{
	char				**args;
	char				*input_fd; // >
	char				*output_fd; // <
	char				*append_output_fd; // >>
	char				*heredoc_fd; // <<
	t_envp				*env_list; // Environment linked list
	int					dollar;
	struct s_command	*next;
}	t_command;

char	*find_env(t_envp *head, char *search);
void	export_add(t_command *iter);

// env
t_envp	*env_head(char **envp);
void	update_env(t_command *iter, char *key, char *new_value);
char	*find_env_value(t_command *iter, char *key);
char	*get_env_value(char *var_name, t_envp *env_list);
void	print_env(t_command *iter);

void	shell_loop(t_envp *env_list);
void	split_built_in(t_command *head);
void	parse_command(char *command_line, t_envp *env_list);
void	parse_dollar(t_command *head);
void	add_node(char **shell, t_envp *env_list);
void	split_built_in(t_command *head);
void	print_command_list(t_command *cmd);
int		ft_strcmp(const char *s1, const char *s2);

// builtin
int	handle_builtin(t_command *command);
int builtin_cd(t_command *iter);
int	builtin_echo(t_command *command);
int	builtin_pwd(void);
int	builtin_unset(t_command *command);
int	builtin_exit(t_command *command, char **args);
int builtin_export(t_command *command);
int builtin_env(t_command *command);

void memory_free(t_command *command);

int error_handler(char *message);
int shell_exit_with_error(char *message, t_command *command);
int shell_exit(t_command *command);

// // Environment linked list functions
// t_envp *create_env_list(char **envp);
// char *find_env_value(t_envp *env_list, char *key);
// void add_env_node(t_envp **head, char *key, char *value);
// void update_env_value(t_envp *env_list, char *key, char *value);
// void free_env_list(t_envp *env_list);

#endif
