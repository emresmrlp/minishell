/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:26:52 by makpolat          #+#    #+#             */
/*   Updated: 2025/05/17 18:30:51 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "../Libft/libft.h"
#include "../include/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdio.h>

void    parse_command(char *comman_line)
{
    char **argv = ft_split(comman_line, ' ');
    //t_command *command;
    
    while (*argv)
    {
        if (ft_strncmp(*argv,"pwd", 4) == 0)
        {
            //char *command = getcwd(NULL, 0);
            printf("%s\n", getcwd(NULL, 0));
        }
        
        printf("parse fonksiyonuna gelen argüman: %s\n", *argv);
        argv++;
    }
    
}