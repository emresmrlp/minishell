/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/07 13:27:48 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
static void built_in(t_command *iter)
{
    if (ft_strcmp(iter->args[0], "pwd") == 0)
    {
        free(iter->args[0]);
        iter->args[0] = getcwd(NULL, 0);
    }
    else if (ft_strcmp(iter->args[0], "echo") == 0) //TODO -n opsiyonu eklenecek
    {
        if (iter->args[1])
            printf("%s\n",iter->args[1]);
        return;
    }
    else if (ft_strcmp(iter->args[0], "cd") == 0)
    {
        //execute_cd(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "export") == 0)
    {
     //   execute_export(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "unset") == 0)
    {
     //   execute_unset(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "env") == 0)
    {
       // execute_env();
    }
    else if (ft_strcmp(iter->args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS); //TODO burayı böyle bırakma leakleri çöz
    }
}

void    split_built_in(t_command *head)
{
    t_command *iter;

    iter = head;
    while (iter)
    {
        if (iter->args && iter->args[0])
        {    
            built_in(iter);
        }
        iter = iter->next;
    }
}
