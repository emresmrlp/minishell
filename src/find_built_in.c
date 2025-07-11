/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:40:47 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/11 17:00:11 by makpolat         ###   ########.fr       */
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

void update_env(t_command *iter, char *key, char *new_value)
{
    t_envp *temp = iter->env_list;
    
    while (temp)
    {
        if (ft_strcmp(temp->key, key) == 0)
        {
            free(temp->value);
            temp->value = ft_strdup(new_value);
            return ;
        }
        temp = temp->next;
    }
    
    t_envp *new_node = (t_envp *)malloc(sizeof(t_envp));
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(new_value);
    new_node->next = iter->env_list;
    iter->env_list = new_node;
}

static void cd_function(t_command *iter)
{
    char *current_pwd = getcwd(NULL, 0);  // Mevcut dizini kaydet
    char *target_dir;
    
    if (!iter->args[1] || ft_strcmp(iter->args[1], "~") == 0)
        target_dir = find_env_value(iter, "HOME");
    else if (ft_strcmp(iter->args[1], "-") == 0)
    {
        target_dir = find_env_value(iter, "OLDPWD");
        if (!target_dir)
        {
            printf("cd: OLDPWD not set\n");
            free(current_pwd);
            return;
        }
    }
    else
        target_dir = iter->args[1];
    if (chdir(target_dir) == -1)
    {
        perror("cd");
        free(current_pwd);
        return;
    }
    update_env(iter, "OLDPWD", current_pwd);
    
    char *new_pwd = getcwd(NULL, 0);
    update_env(iter, "PWD", new_pwd);
    
    free(current_pwd);
    free(new_pwd);
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

void export_add(t_command *iter)
{
    t_envp *temp;
    char *index;

    temp = iter->env_list;
    while (temp->next)
        temp = temp->next;

    temp->next = (t_envp *)malloc(sizeof(t_envp));
    temp->next->next = NULL;
    index = ft_strchr(iter->args[1], '=');
    printf("%s\n", iter->args[1]);
    if (index == 0)
    {
        temp->next->key = ft_strdup(iter->args[1]);
        temp->next->value = NULL;
        return;
    }
    if (index)
    {
        temp->next->key = ft_substr(iter->args[1], 0, index - iter->args[1]);
        temp->next->value = ft_strdup(index + 1);
    }
    else
    {
        temp->next->key = ft_strdup(iter->args[1]);
        temp->next->value = NULL;
    }
}

static void built_in(t_command *iter)
{
    if (ft_strcmp(iter->args[0], "pwd") == 0) //TODO bu ksıma parse gerekebilişr göz atılacak
    {
        printf("%s\n", getcwd(NULL, 0));
        return;
    }
    else if (ft_strcmp(iter->args[0], "echo") == 0) //TODO -n opsiyonu eklenecek //FIXME PİPE VARSA EN SONUNCU ELE ALINACAK
    {
        if (iter->args[1])
            printf("%s\n",iter->args[1]);
        return;
    }
    else if (ft_strcmp(iter->args[0], "cd") == 0) // "-" komutunda sorun var düzeltilecek
    {
        cd_function(iter);
    }
    else if (ft_strcmp(iter->args[0], "export") == 0)
    {
        if (!iter->args[1])
            print_env(iter);
        else
            export_add(iter);
        
    }
    else if (ft_strcmp(iter->args[0], "unset") == 0)
    {
     //   execute_unset(iter->args);
    }
    else if (ft_strcmp(iter->args[0], "env") == 0)
       print_env(iter);
    else if (ft_strcmp(iter->args[0], "exit") == 0)
        exit(EXIT_SUCCESS); //TODO burayı böyle bırakma leakleri çöz
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
