/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/26 19:39:41 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_envp	*env_list;

	if ((ft_strcmp(*argv, "./minishell")) || (argc != 1))
		shell_exit_with_error("MiniShell Wrong program name or "
			"number just run 'minishell'\n", NULL);
	env_list = env_head(envp);
	shell_loop(env_list);
	return (0);
}
