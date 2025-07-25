/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:36:28 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/25 11:53:55 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void free_array(char **array)
{
    int i = 0;

    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char *find_path(char *arg, t_envp *env_list)
{
    char *path;
    char *command_path;
    char *temp;
    char **path_array;
    char **original_path_array;

    path = find_env(env_list, "PATH");
    if (!path)
        return (NULL);
    path_array = ft_split(path, ':');
    if (!path_array)
        return (NULL);
    original_path_array = path_array;
    while (*path_array)
    {
        temp = ft_strjoin(*path_array, "/");
        command_path = ft_strjoin(temp, arg);
        free(temp);
        if (access(command_path, X_OK) == 0)
        {
            free_array(original_path_array);
            return (command_path);
        }
        free(command_path);
        path_array++;
    }
    free_array(original_path_array);
    return (NULL);
}

char **env_list_to_array(t_envp *env_list)
{
    t_envp *current;
    char **env_array;
    int count = 0;
    int i = 0;

    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    env_array = malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);
    current = env_list;
    while (current)
    {
        env_array[i] = ft_strjoin(current->key, "=");
        if (current->value)
            env_array[i] = ft_strjoin(env_array[i], current->value);
        i++;
        current = current->next;
    }
    env_array[i] = NULL;
    return (env_array);
}
