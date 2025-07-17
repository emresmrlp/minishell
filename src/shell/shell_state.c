/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:11:56 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 00:19:28 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int shell_exit_with_error(char *message, t_command *command)
{
    if (message)
        write(2, message, ft_strlen(message));
    memory_free(command);
    exit(FAILURE);
    return (FAILURE);
}

int shell_exit(t_command *command)
{
    memory_free(command);
    exit(SUCCESS);
    return (SUCCESS);
}
