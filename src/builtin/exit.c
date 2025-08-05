/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 10:32:35 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 22:33:03 by ysumeral         ###   ########.fr       */
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

static unsigned long long	parse_number(char *str, int *sign, size_t *i)
{
	unsigned long long	result;

	*i = 0;
	*sign = 1;
	result = 0;
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[(*i)++] == '-')
			*sign = -1;
	}
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		if (result > (ULLONG_MAX - (str[*i] - '0')) / 10)
			return (ULLONG_MAX);
		result = result * 10 + (str[*i] - '0');
		(*i)++;
	}
	return (result);
}

unsigned long long	ft_safe_atol(char *str)
{
	int					sign;
	unsigned long long	result;
	size_t				i;

	result = parse_number(str, &sign, &i);
	if (result == ULLONG_MAX)
		return (ULLONG_MAX);
	if (sign == -1)
	{
		if (result > (unsigned long long)LLONG_MAX + 1)
			return (ULLONG_MAX);
		return ((unsigned long long)((long long)result * -1));
	}
	if (result > LLONG_MAX)
		return (ULLONG_MAX);
	return (result);
}

int	handle_exit_code(char *arg, t_command *command)
{
	unsigned long long	result;
	long long			exit_code;

	printf("exit\n");
	if (is_numeric(arg) == FAILURE)
	{
		error_handler(command, "exit: numeric argument required\n", 0);
		return (2);
	}
	result = ft_safe_atol(arg);
	if (result == ULLONG_MAX)
	{
		error_handler(command, "exit: numeric argument required\n", 0);
		return (2);
	}
	exit_code = (long long)result;
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
		error_handler(command, "exit: too many arguments\n", 0);
		return (FAILURE);
	}
	if (i > 1)
		exit_code = handle_exit_code(args[1], command);
	else
		printf("exit\n");
	clear_history();
	shell_exit(command, exit_code);
	return (exit_code);
}
