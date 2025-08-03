/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:00:53 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/03 21:21:10 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int g_signal_flag = 0;

void	sigint_handler(int signal)
{
	(void)signal;
	if (g_signal_flag == 1)
	{
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
