/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:14:22 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 12:16:51 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_export(t_command *command)
{
    if (!command->args[1])
    {
        print_env(command);
        return (SUCCESS);
    }
    export_add(command);
    return (SUCCESS);
}