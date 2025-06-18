/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/06/18 14:58:46 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void parse_quote_dolar(char ** args)
{
    int quote_count;
    int i;

    i = 0;
    quote_count = 0;
    while (args[i])
    {
        while (args[i] && args[i] == '"')
        {
            quote_count++;
            i++;
        }
    }
    if ((quote_count % 2) != 0)
        return;
    
}

void parse_dollar(t_command *head)
{
    t_command *iter;
    int i;

    i = 0;
    iter = head;
    while (iter->next)
    {
        while (iter->args[i])
        {
            if (ft_strcmp(iter->args[i], '$'))
            {
                parse_quote_dolar(iter->args);
            }
            
        }
        i = 0;
        iter = iter->next;
    }
}
