/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:59:08 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/17 18:59:18 by ysumeral         ###   ########.fr       */
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

void	echo_function(t_command *iter)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (iter->args[i] && n_check(iter->args[i]))
	{
		newline = 0;
		i++;
	}
	while (iter->args[i])
	{
		printf("%s", iter->args[i]);
		if (iter->args[++i])
			printf(" ");
	}
	if (newline)
		printf("\n");
}
