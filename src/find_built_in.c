/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/10 15:40:11 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char *find_env_value(t_command *iter, char *key)
{
    t_envp *temp;

    temp = iter->env_list;
    while (temp)
    {
        if (ft_strcmp(temp->key, key) == 0)
            return (temp->value);
        temp = temp->next;
    }
    return (NULL);
}

static void cd_function(t_command *iter)
{
    if (!iter->args[1] || ft_strcmp(iter->args[1], "~") == 0)
    {
        chdir(find_env_value(iter, "HOME"));
        return ;
    }
    if (ft_strcmp(iter->args[1], "-") == 0)
    {
        //chdir(strrchr(getcwd(NULL, 0), '/'));
        printf("getcwd: %s:\n",getcwd(NULL, 0));
        printf("%s\n", strrchr(getcwd(NULL, 0), '/'));
    }
    if (chdir(iter->args[1]) == -1)
    {
        perror("cd");
        return;
    }
}



static void print_env(t_command *iter)
{
    t_envp *temp;
    
    temp = iter->env_list;
    while (temp)
    {
        if (temp->key && temp->value)
            printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}
static void built_in(t_command *iter)
{
    if (ft_strcmp(iter->args[0], "pwd") == 0) //TODO bu ksıma parse gerekebilişr göz atılacak
    {
        free(iter->args[0]);
        iter->args[0] = getcwd(NULL, 0);
        printf("%s\n", iter->args[0]);
        return;
    }
    else if (ft_strcmp(iter->args[0], "echo") == 0) //TODO -n opsiyonu eklenecek
    {
        if (iter->args[1])
            printf("%s\n",iter->args[1]);
        return;
    }
    else if (ft_strcmp(iter->args[0], "cd") == 0)
    {
        cd_function(iter);
    }
    else if (ft_strcmp(iter->args[0], "export") == 0)
    {
        //export_function(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "unset") == 0)
    {
     //   execute_unset(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "env") == 0)
    {
       print_env(iter);
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
