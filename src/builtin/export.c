/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:14:22 by ysumeral          #+#    #+#             */
/*   Updated: 2025/07/18 23:33:34 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TODO - export env listesini alacak.
// TODO - başına declare -x eklenecek.
// TODO - alfabetik olarak bubble algoritmasıyla sıralanacak.
// TODO - value NULL olanlar env'de listelenmeyip export'ta listelenecek.
// TODO - mevcut kodda AD= yada AD gibi girdiler kabul edilmiyor, düzeltilecek ve bu tarz girdilerde value NULL olarak ayarlanacak.

static void export_add(t_command *iter)
{
	t_envp	*temp;
	char	*index;

	temp = iter->env_list;
	while (temp->next)
		temp = temp->next;
	temp->next = (t_envp *)malloc(sizeof(t_envp));
	temp->next->next = NULL;
	index = ft_strchr(iter->args[1], '=');
	if (index)
	{
		temp->next->key = ft_substr(iter->args[1], 0, index - iter->args[1]);
		temp->next->value = ft_strdup(index + 1);
	}
}

static int is_valid_key(char *arg)
{
    int key_len;
    int i;
    char *value;

    i = 1;
    value = ft_strchr(arg, '=');
    if (!value)
        return (FAILURE);
    key_len = value - arg;
    if (!arg || arg[0] == '\0')
        return (FAILURE);
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (FAILURE);
    while (arg[i] && i < key_len)
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (FAILURE);
        i++;
    }
    return (SUCCESS);
}

static int is_valid_var(char *var)
{
    char *value;

    if (!var || !is_valid_key(var))
        return (FAILURE);
    value = ft_strchr(var, '=');
    if (!value)
        return (FAILURE);
    if (*(value + 1) == '\0')
        return (FAILURE);
    return (SUCCESS);
}

static int handle_export(t_command *command, char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        if (is_valid_var(args[i]))
            export_add(command);
        else
            error_handler("export: not a valid identifier");
        i++;
    }
    return (SUCCESS);
}

int builtin_export(t_command *command, char **args)
{
    if (args[1] == NULL)
    {
        print_env(command);
        return (SUCCESS);
    }
    if (handle_export(command, args))
        return (SUCCESS);
    else
        return (FAILURE);
}