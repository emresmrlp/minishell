/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 10:32:35 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 12:37:10 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (FAILURE);
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	handle_exit_code(char *arg)
{
	int exit_code;

	exit_code = 1;
	if (is_numeric(arg) == FAILURE)
		return (exit_code);
	return (exit_code);
}

int	builtin_exit(t_command *command, char **args)
{
	int	exit_code;
	int i;
	
	i = 0;
	exit_code = SUCCESS;
	while (args[i])
		i++;
	if (i > 2)
	{
		error_handler("exit: too many arguments\n");
		return (FAILURE);
	}
	if (i > 1)
	{
		exit_code = handle_exit_code(args[1]);
		if (exit_code == FAILURE)
		{
			error_handler("exit: numeric argument required\n");
			return (FAILURE);
		}
	}
	printf("exit\n");
	clear_history();
	shell_exit(command);
	return (exit_code);
}
