/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:34:59 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/25 11:53:51 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;
	char	*path;
	char	**env_list_array;

	pid = fork();
	if (pid == 0)
	{
		path = find_path(command->args[0], command->env_list);
		env_list_array = env_list_to_array(command->env_list);
		if (execve(path, command->args, env_list_array) == -1)
			printf("minishell: command not found\n");
		exit(127);
	}
	else
		waitpid(pid, &status, 0);
}

/*static void execute_pipeline(t_command *command)
{
	(void)command;
}*/

static int	is_builtin(const char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute(t_command *command)
{
	if (!command)
		return ;
	if (!command->next)
	{
		if (is_builtin(command->args[0]))
			handle_builtin(command);
		else
			execute_single(command);
		return ;
	}
	else
		handle_builtin(command);
}
