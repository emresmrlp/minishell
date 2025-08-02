/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/02 20:49:32 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int command->exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_envp	*env_list;

	(void)argv;
	if (argc != 1)
		shell_exit_with_error("minishell: arguments not allowed\n", NULL);
	env_list = env_head(envp);
	shell_loop(env_list);
	return (command->exit_status);
}
