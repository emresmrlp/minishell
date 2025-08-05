/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:00:00 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_variable(t_expansion_data *data, char *line,
	t_envp *env_list)
{
	data->j = data->i + 1;
	while (line[data->j] && (ft_isalnum(line[data->j])
			|| line[data->j] == '_'))
		data->j++;
	data->temp = ft_substr(line, data->i + 1,
			data->j - data->i - 1);
	if (data->temp)
	{
		data->env_value = get_env_value(NULL, data->temp, env_list);
		data->old_result = data->result;
		if (data->env_value)
			data->result = ft_strjoin(data->result, data->env_value);
		else
			data->result = ft_strjoin(data->result, "");
		free(data->old_result);
		free(data->temp);
	}
	data->i = data->j;
}

char	*expand_heredoc_line(char *line, t_envp *env_list)
{
	t_expansion_data	data;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	data.result = ft_strdup("");
	data.i = 0;
	while (line[data.i])
	{
		if (line[data.i] == '$' && line[data.i + 1]
			&& (ft_isalnum(line[data.i + 1])
				|| line[data.i + 1] == '_'))
			expand_variable(&data, line, env_list);
		else
		{
			data.temp = ft_substr(line, data.i, 1);
			data.old_result = data.result;
			data.result = ft_strjoin(data.result, data.temp);
			free(data.old_result);
			free(data.temp);
			data.i++;
		}
	}
	return (data.result);
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
