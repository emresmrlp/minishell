/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:22:03 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 19:29:27 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	n_check(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] == '\0')
		return (0);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

int	builtin_echo(t_command *command)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (command->args[i] && n_check(command->args[i]))
	{
		newline = 0;
		i++;
	}
	while (command->args[i])
	{
		if (i > 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(command->args[i], STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}
