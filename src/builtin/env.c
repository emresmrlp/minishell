/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:26:24 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 12:26:58 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_env(t_command *command)
{
    if (!command->env_list)
    {
        error_handler("env: no environment variables set\n");
        return (FAILURE);
    }
    print_env(command);
    return (SUCCESS);
}