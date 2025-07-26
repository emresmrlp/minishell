/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 18:02:16 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/26 23:37:24 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char  **sort_array(char **env_array, int size)
{
    char  *temp;
    int     i;
    int     j;

    if (!env_array || size <= 0)
        return (NULL);
    i = 0;
    while (i < size - 1)
    {
        j = 0;
        while (j < size - i - 1)
        {
            if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
            {
                temp = env_array[j];
                env_array[j] = env_array[j + 1];
                env_array[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
    return (env_array);
}

char **env_list_to_array(t_envp *env_list)
{
    t_envp  *temp;
    char  **env_array;
    int     size;
    int     i;

    size = get_env_size(env_list);
    i = 0;
    env_array = malloc(sizeof(char *) * (size + 1));
    if (!env_array)
        return (NULL);
    temp = env_list;
    while (temp)
    {
        env_array[i] = ft_strjoin(temp->key, "=");
        if (temp->value)
            env_array[i] = ft_strjoin(env_array[i], temp->value);
        i++;
        temp = temp->next;
    }
    env_array[i] = NULL;
    return (env_array);
}
