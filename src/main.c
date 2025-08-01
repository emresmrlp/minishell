/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/01 14:43:29 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_envp	*env_list;

	(void)argv;
	if (argc != 1)
		shell_exit_with_error("minishell: arguments not allowed\n", NULL);
	env_list = env_head(envp);
	shell_loop(env_list);
	return (g_exit_status);
}
