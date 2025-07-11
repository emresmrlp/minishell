/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/10 19:41:26 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../Libft/libft.h"

void error(char *message)
{
	write(2, message, ft_strlen(message));
	exit(EXIT_FAILURE);
}

void read_input(t_envp *env_list)
{
	char *command_line;
	while (1)
	{
		command_line = readline("MiniShell: ");
		if (command_line && *command_line)
		{
			add_history(command_line);
			parse_command(command_line, env_list);
			if (!ft_strncmp(command_line, "exit", ft_strlen(command_line))) //TODO bu komut built-in kısmına taşınacak ve leaklere bakılacak ondan sonra
			{
				free(command_line);
				clear_history();
				break ;
			}
		}
		free(command_line);
	}
}

char *find_env(t_envp *head, char *search)
{
	t_envp *temp;

	temp = head;
	while (temp)
	{
		if (ft_strcmp(temp->key, search) == 0)
		{
			free(temp->key);
			return (temp->value);
		}
		temp = temp->next;
	}
	return (NULL);
}
t_envp  *env_head(char ** envp)
{
	t_envp *temp;
	t_envp *head;
	int i;

	i = 0;
	head = (t_envp *)malloc(sizeof(t_envp));
	head->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
	head->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
	temp = head;
	while (envp[i])
	{
		temp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		temp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (envp[i + 1])
		{
			temp->next = (t_envp *)malloc(sizeof(t_envp));
			if (!temp->next)
			error("Malloc Error\n");
			temp = temp->next;
		}
		else
		temp->next = NULL;
		i++;	
	}
	return (head);
}

int	main(int argc, char **argv, char **envp)
{
	// Environment linked list'i oluştur
	t_envp *env_list;
	
	env_list = env_head(envp);
	
	(void)argc;
	if ((ft_strcmp(*argv,"./minishell")) || (argc != 1))
		error("Wrong program name or number just run 'minishell'\n");

	read_input(env_list);
	
	// Temizlik
	//free_env_list(env_list);

	return (0);
}