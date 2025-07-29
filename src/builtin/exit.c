/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 10:32:35 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 15:05:51 by makpolat         ###   ########.fr       */
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

unsigned long long	ft_safe_atol(char *str)
{
	size_t				i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += (str[i] - 48);
		i++;
	}
	if (result > 9223372036854775807ULL && sign == 1)
		return (10000000000000000000ULL);
	if (result < 9223372036854775808ULL && sign == -1)
		return (10000000000000000000ULL);
	return (result * sign);
}

int	handle_exit_code(char *arg)
{
	int	exit_code;

	printf("exit\n");
	if (is_numeric(arg) == FAILURE || ft_safe_atol(arg) > LONG_MAX)
	{
		error_handler("exit: numeric argument required\n");
		return (255);
	}
	exit_code = (ft_safe_atol(arg) % 256);
	return (exit_code);
}

int	builtin_exit(t_command *command, char **args)
{
	int	exit_code;
	int	i;

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
		exit_code = handle_exit_code(args[1]);
	else
		printf("exit\n");
	clear_history();
	shell_exit(command, exit_code);
	return (exit_code);
}
