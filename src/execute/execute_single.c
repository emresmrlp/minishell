/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:49 by ysumeral          #+#    #+#             */
/*   Updated: 2025/08/02 15:44:06 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;
	int		i;
	int		write_idx;
	int		total_args;

	/* Args kontrolü - eğer komut yoksa hata */
	if (!command->args || !command->args[0])
	{
		error_handler("minishell: syntax error near unexpected token\n");
		g_exit_status = 2;
		return ;
	}
	/* Boş argument'ları free et ve compact et */
	i = 0;
	write_idx = 0;
	total_args = 0;
	/* Count total args first */
	while (command->args[total_args])
		total_args++;
	/* First pass: free empty strings */
	for (i = 0; i < total_args; i++)
	{
		if (command->args[i] && command->args[i][0] == '\0')
		{
			free(command->args[i]);
			command->args[i] = NULL;
		}
	}
	/* Second pass: compact array (remove NULLs) */
	write_idx = 0;
	for (i = 0; i < total_args; i++)
	{
		if (command->args[i] != NULL)
		{
			if (i != write_idx)
				command->args[write_idx] = command->args[i];
			write_idx++;
		}
	}
	command->args[write_idx] = NULL; /* Terminate compacted array */
	/* Hiç valid command yok */
	if (!command->args[0])
	{
		g_exit_status = 0;
		return ;
	}

	if (is_builtin(command->args[0]))
	{
		// Shell state'ini değiştiren komutlar - fork kullanma
		if (ft_strcmp(command->args[0], "exit") == 0)
		{
			g_exit_status = builtin_exit(command, command->args);
			return ;
		}
		if (ft_strcmp(command->args[0], "cd") == 0)
		{
			g_exit_status = builtin_cd(command);
			return ;
		}
		if (ft_strcmp(command->args[0], "export") == 0)
		{
			g_exit_status = builtin_export(command, command->args);
			return ;
		}
		if (ft_strcmp(command->args[0], "unset") == 0)
		{
			builtin_unset(command, command->args);
			g_exit_status = SUCCESS;
			return ;
		}
		/*
		** Sadece output üreten builtin komutlar için fork kullan
		** (pwd, echo, env - redirection desteği için)
		*/
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		pid = fork();
		if (pid == 0)
		{
			execute_redirection(command);
			exit(execute_builtin(command));
		}
		else if (pid < 0)
		{
			perror("fork");
			close(saved_stdin);
			close(saved_stdout);
			return ;
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
		/* File descriptorleri eski haline getir */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		execute_redirection(command);
		execute_command(command);
		exit(127);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
}
