/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/17 19:16:21 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error(char *message)
{
	write(2, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	t_envp	*env_list;

	env_list = env_head(envp);
	if ((ft_strcmp(*argv, "./minishell")) || (argc != 1))
		error("MiniShell Wrong program name or number just run 'minishell'\n");
	read_input(env_list);
	//free_env_list(env_list);
	return (0);
}
