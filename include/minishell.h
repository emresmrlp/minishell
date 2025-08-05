/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:51:50 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 21:08:15 by makpolat         ###   ########.fr       */
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

extern int	g_signal_flag;

typedef struct s_envp
{
	char			*key;
	char			*value;
	int				is_temporary;
	struct s_envp	*next;
}	t_envp;

typedef struct s_parse_data
{
	char	*result;
	int		*i;
	int		*j;
	int		*state;
}	t_parse_data;

typedef struct s_heredoc_data
{
	int		temp_fd;
	char	*line;
	char	*expanded_line;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);
	char	temp_filename[256];
}	t_heredoc_data;

typedef struct s_expansion_data
{
	char	*result;
	char	*temp;
	char	*env_value;
	char	*old_result;
	int		i;
	int		j;
}	t_expansion_data;

typedef struct s_file_creation_data
{
	int		i;
	int		fd;
	char	*error_msg;
	char	*final_msg;
}	t_file_creation_data;

typedef struct s_command
{
	int					exit_status;
	char				**args;
	int					argc;
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
int			is_shell_state_builtin(const char *cmd);
t_envp		*env_head(char **envp);
char		*find_env(t_envp *head, char *search);
void		update_env(t_command *iter, char *key, char *new_value);
char		*find_env_value(t_command *iter, char *key);
char		*get_env_value(t_command *command, char *var_name,
				t_envp *env_list);
void		remove_env(t_command *command, char *key);
void		shell_loop(t_envp *env_list);
void		split_built_in(t_command *head);
void		parse_command(char *command_line, t_envp *env_list);
void		parse_dollar(t_command *head);
int			pipe_count(const char *line);
void		handle_quotes_in_pipe(const char c, int *in_single, int *in_double);
void		add_pipe_segment(const char *line, char **shell, int *vars);
void		free_shell_array(char **shell);
void		finalize_pipe_split(const char *line, char **shell, int *vars);
void		add_to_fd_array(char ***fd_array, char *filename);
int			has_single_quotes(char *str);
int			calculate_buffer_size(char *str, char delimiter);
void		free_tokens(char **tokens);
char		*space_strjoin_function(char **arr);
char		**split_with_quotes_preserved(char *str, char delimiter);
int			validate_redirection(char **tokens, int index, t_command *command);
void		handle_input_redirect(t_command *node, char **tokens, int *j);
void		handle_output_redirect(t_command *node, char **tokens, int *j);
void		handle_redirection(t_command *node, char **tokens, int *j);
void		process_redirection_token(t_command *node, char **tokens,
				char **original_tokens, int *index);
void		process_regular_token(t_command *node, char **tokens,
				char **original_tokens, int *index);
int			is_redirection(char *token);
int			ft_arg_count(char **tokens, char **original_tokens,
				t_command *command);
int			parse_argv(t_command *node, char **tokens, char *original_string);
void		handle_redirection(t_command *node, char **tokens, int *j);
char		**split_redirects_for_command(char *command);
t_command	*process_single_command(char *shell_command, t_envp *env_list);
void		process_redirection_token(t_command *node, char **tokens,
				char **original_tokens, int *index);
void		process_regular_token(t_command *node, char **tokens,
				char **original_tokens, int *index);
int			is_token_quoted(char **original_tokens, int index);
int			is_redirection(char *token);
int			is_var_char(char c);
char		*expand_var(char *str, int *i, t_envp *env_list);
void		handle_quotes(char c, int *in_single, int *in_double);
char		*get_expanded_variable(char *str, int *i, t_envp *env_list);
void		append_text_to_result(char **result, int *j, char *text);
int			expansion_check(char *str);
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
void		export_add(t_command *iter, int arg_index);
void		export_update(t_command *iter, int arg_index, char *key);
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

int			count_words(const char *s, char c);
char		**free_split_array(char **arr);
void		remove_quotes_from_word(const char *s, char c, char *result);

void		remove_empty_strings(t_command *command, int total_args);
void		compact_args_array(t_command *command, int total_args);
void		setup_child_builtin_signals_and_wait(t_command *command,
				pid_t pid, int saved_stdin, int saved_stdout);
void		setup_external_signals_and_wait(t_command *command, pid_t pid);
int			handle_empty_command(t_command *command);
void		process_builtin_with_fork(t_command *command);
void		process_external_command(t_command *command);
void		handle_shell_state_builtins(t_command *command);

// Multiple execution functions
void		setup_child_fds(int prev_fd, int write_fd);
void		close_extra_fds(void);
void		child_process(t_command *command, int prev_fd, int write_fd);
pid_t		fork_and_run_child(t_command *command, int prev_fd, int write_fd);

// Redirection functions
char		*expand_heredoc_line(char *line, t_envp *env_list);
char		*get_last_element(char **array);
int			redirect_append_simple(char *filename, t_command *command);
int			redirect_output_simple(char *filename, t_command *command);
int			redirect_heredoc(char *delimiter, t_envp *env_list);
void		handle_input_redirection(t_command *command);
void		handle_heredoc_redirection(t_command *command);

// Utility functions for replacing non-allowed functions
int			get_pid_from_proc(void);
void		create_temp_filename(char *filename);
void		int_to_string(int num, char *str);

#endif