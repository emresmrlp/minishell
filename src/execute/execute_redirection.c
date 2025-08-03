/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:36:38 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 16:06:06 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	redirect_input_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": No such file or directory\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_output_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": Permission denied\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_append_simple(char *filename, t_command *command)
{
	int		fd;
	char	*error_msg;
	char	*final_msg;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		error_msg = ft_strjoin("minishell: ", filename);
		final_msg = ft_strjoin(error_msg, ": Permission denied\n");
		error_handler(command, final_msg, 1);
		free(error_msg);
		free(final_msg);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		error_handler(command, "minishell: dup2 failed\n", 1);
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
	exit(130);
}

static char	*expand_heredoc_line(char *line, t_envp *env_list)
{
	char	*result;
	char	*temp;
	char	*env_value;
	char	*old_result;
	int		i;
	int		j;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && 
			(ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			j = i + 1;
			while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
				j++;
			temp = ft_substr(line, i + 1, j - i - 1);
			if (temp)
			{
				env_value = get_env_value(NULL, temp, env_list);
				old_result = result;
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
			old_result = result;
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
	int		temp_fd;
	char	*line;
	char	*expanded_line;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);
	char	temp_filename[256];

	/* Temporary file oluştur */
	sprintf(temp_filename, "/tmp/minishell_heredoc_%d", getpid());
	temp_fd = open(temp_filename, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd < 0)
		return (-1);
	
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
		write(temp_fd, expanded_line, ft_strlen(expanded_line));
		write(temp_fd, "\n", 1);
		free(line);
		free(expanded_line);
	}
	
	/* File'ı başa sar okuma için */
	lseek(temp_fd, 0, SEEK_SET);
	
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	
	/* Temp file'ı sil (fd hala açık olacak) */
	unlink(temp_filename);
	
	return (temp_fd);
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
static int create_previous_files(char **files, int is_append, t_command *command)
{
	int i = 0;
	int fd;
	char *error_msg;
	
	if (!files)
		return (0);
	
	// Önce tüm dosyaları kontrol et (son dahil)
	while (files[i])
	{
		if (is_append)
			fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		else
			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd >= 0)
			close(fd);
		else
		{
			error_msg = ft_strjoin("minishell: ", files[i]);
			char *final_msg = ft_strjoin(error_msg, ": Permission denied\n");
			error_handler(command, final_msg, 1);
			free(error_msg);
			free(final_msg);
			return (1); // Hata varsa dur
		}
		i++;
	}
	return (0);
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
				command->exit_status = 130;
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
		// Önce input dosyalarını kontrol et - hata varsa dur
		if (command->input_fd)
		{
			i = 0;
			while (command->input_fd[i])
			{
				if (redirect_input_simple(command->input_fd[i], command) != 0)
					exit(command->exit_status);
				i++;
			}
		}
	}

	// Sonra tüm output redirection'ları kontrol et - hata varsa dur
	if (command->output_fd && create_previous_files(command->output_fd, 0, command) != 0)  // Output files (truncate)
		exit(command->exit_status);
	if (command->append_fd && create_previous_files(command->append_fd, 1, command) != 0)  // Append files (no truncate)
		exit(command->exit_status);
	
	// Output redirection'ları uygula
	last_append = get_last_element(command->append_fd);
	last_output = get_last_element(command->output_fd);
	
	if (last_append)
	{
		if (redirect_append_simple(last_append, command) != 0)
			exit(command->exit_status);
	}
	else if (last_output)
	{
		if (redirect_output_simple(last_output, command) != 0)
			exit(command->exit_status);
	}
}
