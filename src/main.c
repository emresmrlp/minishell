/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/17 15:09:27 by makpolat         ###   ########.fr       */
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
	t_envp	*env_list; // Environment linked list'i oluştur

	env_list = env_head(envp);
	if ((ft_strcmp(*argv, "./minishell")) || (argc != 1))
		error("Wrong program name or number just run 'minishell'\n");
	read_input(env_list);
	// Temizlik 
	//free_env_list(env_list);
	return (0);
}
