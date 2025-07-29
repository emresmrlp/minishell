/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 10:32:35 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/29 15:58:17 by makpolat         ###   ########.fr       */
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
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (ULLONG_MAX - (str[i] - '0')) / 10)
			return (ULLONG_MAX); // Overflow
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (sign == -1)
	{
		if (result > (unsigned long long)LLONG_MAX + 1)
			return (ULLONG_MAX); // Overflow
		return ((unsigned long long)((long long)result * -1));
	}
	if (result > LLONG_MAX)
		return (ULLONG_MAX); // Overflow
	return (result);
}

int	handle_exit_code(char *arg)
{
	unsigned long long	result;
	long long			exit_code;

	printf("exit\n");
	if (is_numeric(arg) == FAILURE)
	{
		error_handler("exit: numeric argument required\n");
		return (2); // Bash uses 2 for non-numeric
	}
	
	result = ft_safe_atol(arg);
	if (result == ULLONG_MAX)
	{
		error_handler("exit: numeric argument required\n");
		return (2);
	}
	
	// Convert to signed for proper modulo calculation
	exit_code = (long long)result;
	
	// Bash-compatible exit code: proper modulo 256 handling
	return ((int)((exit_code % 256 + 256) % 256));
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
		printf("exit\n");
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
