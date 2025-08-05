/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:36:28 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 17:54:23 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	get_pid_from_proc(void)
{
	int		fd;
	char	buffer[256];
	int		bytes_read;
	int		pid;
	int		i;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
		return (-1);
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes_read <= 0)
		return (-1);
	buffer[bytes_read] = '\0';
	pid = 0;
	i = 0;
	while (buffer[i] && buffer[i] != ' ')
	{
		if (buffer[i] >= '0' && buffer[i] <= '9')
			pid = pid * 10 + (buffer[i] - '0');
		i++;
	}
	return (pid);
}

void	int_to_string(int num, char *str)
{
	int		i;
	int		temp;
	int		len;

	if (num == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return ;
	}
	temp = num;
	len = 0;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	i = len - 1;
	while (num > 0)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
		i--;
	}
	str[len] = '\0';
}

void	create_temp_filename(char *filename)
{
	char	*base;
	int		i;
	char	pid_str[20];
	int		j;

	base = "/tmp/minishell_heredoc_";
	i = 0;
	while (base[i])
	{
		filename[i] = base[i];
		i++;
	}
	int_to_string(get_pid_from_proc(), pid_str);
	j = 0;
	while (pid_str[j])
	{
		filename[i] = pid_str[j];
		i++;
		j++;
	}
	filename[i] = '\0';
}
