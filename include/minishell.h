/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 22:46:06 by ysumeral         ###   ########.fr       */
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

typedef struct s_envp
{
	char			*key;
	char			*value;
	int				is_temporary;
	struct s_envp	*next;
}	t_envp;

typedef struct s_command
{
	int					exit_status;
	char				**args;
	char				**input_fd;
	char				**output_fd;
	char				**append_fd;
	char				**heredoc_fd;
	t_envp				*env_list;
	int					dollar;
	int					*skip_expansion;
	struct s_command	*next;
}	t_command;

void		sigint_handler(int signal);
void		execute_redirection(t_command *command);
int			execute_builtin_redirection(t_command *command);
void		execute(t_command *command);
void		execute_command(t_command *command);
void		execute_single(t_command *command);
void		execute_multiple(t_command *command);
char		*get_valid_path(t_command *command);
int			is_builtin(const char *cmd);
t_envp		*env_head(char **envp);
char		*find_env(t_envp *head, char *search);
void		update_env(t_command *iter, char *key, char *new_value);
char		*find_env_value(t_command *iter, char *key);
char		*get_env_value(t_command *command, char *var_name, t_envp *env_list);
void		remove_env(t_command *command, char *key);
void		shell_loop(t_envp *env_list);
void		split_built_in(t_command *head);
void		parse_command(char *command_line, t_envp *env_list);
void		parse_dollar(t_command *head);
int			is_var_char(char c);
char		*my_strdup(char *src);
char		*expand_var(char *str, int *i, t_envp *env_list);
void		handle_quotes(char c, int *in_single, int *in_double);
void		append_expansion(char **result, int *j, char *str, int *i,
				t_envp *env_list, int buffer_size);
int			has_expansion(char *str);
t_command	*add_node(char **shell, t_envp *env_list);
void		print_command_list(t_command *cmd);
int			ft_strcmp(const char *s1, const char *s2);
char		**redirect_split(char **shell);
int			inside_quote(char *str, int pos);
int			token_count_redirect(char *token, int *j, int *found_redirect);
int			process_token_char(char *token, int *j, int *find, int *count);
int			find_next_redirect(char *str, int start);
int			find_redirection_pos(char *token);
int			execute_builtin(t_command *command);
int			builtin_cd(t_command *iter);
int			builtin_echo(t_command *command);
int			builtin_pwd(void);
void		builtin_unset(t_command *command, char **args);
int			builtin_exit(t_command *command, char **args);
int			builtin_export(t_command *command, char **args);
int			builtin_env(t_command *command);
char		*handle_exit_status(t_command *command, t_envp *env_list);
void		memory_free(t_command *command);
char		**sort_array(char **env_array, int size);
char		**env_list_to_array(t_envp *env_list, int size);
void		set_node_values(t_envp *new_node, char *arg, char *index);
int			process_export_arg(t_command *command, char *arg, int i);
char		*find_path(char *arg, t_command *command);
void		free_array(char **array);
int			is_valid_key(char *arg);
void		cleanup_empty_env_vars(t_envp **env_list);
int			get_env_size(t_envp *env_list);
void		cleanup_exit_status_str(t_command *command);
void		save_exit_status_to_env(t_envp *env_list, int exit_status);
int			error_handler(t_command *command, char *message, int exit_code);
int			shell_exit_with_error(char *message, t_command *command);
int			shell_exit(t_command *command, int exit_code);
void		free_env_list(t_envp *env_list);

#endif