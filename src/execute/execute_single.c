/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:33:49 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/30 18:58:08 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_single(t_command *command)
{
	int		status;
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;

	// Args kontrolü - eğer komut yoksa hata
	if (!command->args || !command->args[0])
	{
		error_handler("minishell: syntax error near unexpected token\n");
		g_exit_status = 2;
		return ;
	}

	if (is_builtin(command->args[0]))
	{
		// Exit komutu için özel durum - fork kullanma
		if (ft_strcmp(command->args[0], "exit") == 0)
		{
			g_exit_status = builtin_exit(command, command->args);
			return ;
		}
		
		// Diğer builtin komutlar için
		// Orijinal file descriptorleri kaydet
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		
		// Redirection error handling için fork kullan
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
		
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		
		// File descriptorleri eski haline getir
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
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}
