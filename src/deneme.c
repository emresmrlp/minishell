/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deneme.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/05/12 12:30:49 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdio.h>


int main(void)
{
    char *first;
 
    while ((first = readline("MiniShell: ")))
    {
        if (*first)
        {
            add_history(first);
        }
        
        printf("Girilen komut: %s\n",first);
        if (strcmp(first,"exit") == 0)
        {
            break;
        }
        
    }
    
        
        
}