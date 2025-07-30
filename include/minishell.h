/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/30 18:05:17 by makpolat         ###   ########.fr       */
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
# include <sys/stat.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>

# define SUCCESS 0
# define FAILURE 1

extern int g_exit_status;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_command
{
	char				**args;
	char				*input_fd;
	char				*output_fd;
	char				*append_fd;
	char				*heredoc_fd;
	t_envp				*env_list;
	int					dollar;
	int					*skip_expansion;
	struct s_command	*next;
}	t_command;

void	sigint_handler(int signal);
void	execute_redirection(t_command *command);
int		execute_builtin_redirection(t_command *command);
void	execute(t_command *command);
void	execute_command(t_command *command);
void	execute_single(t_command *command);
void	execute_multiple(t_command *command);
char	*get_valid_path(t_command *command);
int		is_builtin(const char *cmd);
t_envp	*env_head(char **envp);
char	*find_env(t_envp *head, char *search);
void	update_env(t_command *iter, char *key, char *new_value);
char	*find_env_value(t_command *iter, char *key);
char	*get_env_value(char *var_name, t_envp *env_list);
void	remove_env(t_command *command, char *key);
void	shell_loop(t_envp *env_list);
void	split_built_in(t_command *head);
void	parse_command(char *command_line, t_envp *env_list);
void	parse_dollar(t_command *head);
void	add_node(char **shell, t_envp *env_list);
void	print_command_list(t_command *cmd);
int		ft_strcmp(const char *s1, const char *s2);
char	**redirect_split(char **shell);
int		execute_builtin(t_command *command);
int		builtin_cd(t_command *iter);
int		builtin_echo(t_command *command);
int		builtin_pwd(void);
void	builtin_unset(t_command *command, char **args);
int		builtin_exit(t_command *command, char **args);
int		builtin_export(t_command *command, char **args);
int		builtin_env(t_command *command);
void	memory_free(t_command *command);
char	**sort_array(char **env_array, int size);
char	**env_list_to_array(t_envp *env_list, int size);
char	*find_path(char *arg, t_command *command);
int		is_valid_key(char *arg);
int		get_env_size(t_envp *env_list);
char	*get_env_value(char *var_name, t_envp *env_list);
int		error_handler(char *message);
int		shell_exit_with_error(char *message, t_command *command);
int		shell_exit(t_command *command, int exit_code);

#endif