/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 17:08:35 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	redirect_input_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 input");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_output_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 output");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_append_simple(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 append");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static void	heredoc_sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	/* Exit the readline call */
	exit(130);
}

static char	*expand_heredoc_line(char *line, t_envp *env_list)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			j = i + 1;
			while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
				j++;
			temp = ft_substr(line, i + 1, j - i - 1);
			if (temp)
			{
				char *env_value = get_env_value(temp, env_list);
				char *old_result = result;
				if (env_value)
					result = ft_strjoin(result, env_value);
				else
					result = ft_strjoin(result, "");
				free(old_result);
				free(temp);
			}
			i = j;
		}
		else
		{
			temp = ft_substr(line, i, 1);
			char *old_result = result;
			result = ft_strjoin(result, temp);
			free(old_result);
			free(temp);
			i++;
		}
	}
	return (result);
}

static int	redirect_heredoc(char *delimiter, t_envp *env_list)
{
	int		heredoc_fd[2];
	char	*line;
	char	*expanded_line;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	pipe(heredoc_fd);
	old_sigint = signal(SIGINT, heredoc_sigint_handler);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		expanded_line = expand_heredoc_line(line, env_list);
		write(heredoc_fd[1], expanded_line, ft_strlen(expanded_line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
		free(expanded_line);
	}
	close(heredoc_fd[1]);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	return (heredoc_fd[0]);
}

// Son elemanı bul (array'in sonuncusu aktif redirection)
static char *get_last_element(char **array)
{
	int i = 0;
	
	if (!array)
		return NULL;
	
	while (array[i])
		i++;
	
	if (i > 0)
		return array[i - 1];
	
	return NULL;
}

// Array'daki son eleman hariç tüm dosyaları oluştur
static void create_previous_files(char **files, int is_append)
{
	int i = 0;
	int fd;
	
	if (!files)
		return;
	
	// Son eleman hariç tüm dosyaları oluştur
	while (files[i + 1])  // Son eleman (NULL) değilse devam et
	{
		if (is_append)
			fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		else
			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd >= 0)
			close(fd);
		else
			perror("minishell");
		i++;
	}
}

void	execute_redirection(t_command *command)
{
	int	heredoc_fd;
	int	i;
	char *last_output, *last_append;

	// Heredoc'ları sırayla işle (tüm heredoc'ları consume et, son olanını kullan)
	if (command->heredoc_fd)
	{
		i = 0;
		while (command->heredoc_fd[i])
		{
			heredoc_fd = redirect_heredoc(command->heredoc_fd[i], command->env_list);
			if (heredoc_fd == -1)
			{
				g_exit_status = 130;
				return;
			}
			// Sadece son heredoc'u stdin'e yönlendir
			if (!command->heredoc_fd[i + 1])  // Son heredoc
			{
				dup2(heredoc_fd, STDIN_FILENO);
			}
			close(heredoc_fd);
			i++;
		}
	}
	else
	{
		// Tüm input dosyalarını sırayla kontrol et ve açmaya çalış
		if (command->input_fd)
		{
			i = 0;
			while (command->input_fd[i])
			{
				if (redirect_input_simple(command->input_fd[i]) != 0)
					exit(1);
				i++;
			}
		}
	}
	
	// Önceki dosyaları boş oluştur
	create_previous_files(command->output_fd, 0);  // Output files (truncate)
	create_previous_files(command->append_fd, 1);  // Append files (no truncate)
	
	// Son redirection'ı aktif olarak kullan
	last_append = get_last_element(command->append_fd);
	last_output = get_last_element(command->output_fd);
	
	if (last_append)
	{
		if (redirect_append_simple(last_append) != 0)
			exit(1);
	}
	else if (last_output)
	{
		if (redirect_output_simple(last_output) != 0)
			exit(1);
	}
}
