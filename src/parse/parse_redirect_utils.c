/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:09:05 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 15:15:11 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_fd_array_size(char **fd_array)
{
	int	count;

	count = 0;
	if (fd_array)
	{
		while (fd_array[count])
			count++;
	}
	return (count);
}

static void	copy_fd_array(char **new_array, char **old_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_array[i] = old_array[i];
		i++;
	}
}

void	add_to_fd_array(char ***fd_array, char *filename)
{
	int		count;
	char	**new_array;

	if (!filename)
		return ;
	count = count_fd_array_size(*fd_array);
	new_array = malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return ;
	copy_fd_array(new_array, *fd_array, count);
	new_array[count] = ft_strdup(filename);
	new_array[count + 1] = NULL;
	if (*fd_array)
		free(*fd_array);
	*fd_array = new_array;
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	*space_strjoin_function(char **arr)
{
	int		i;
	char	*result;
	char	*tmp;

	if (!arr || !arr[0])
		return (NULL);
	result = ft_strdup(arr[0]);
	i = 1;
	while (arr[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		tmp = result;
		result = ft_strjoin(result, arr[i]);
		free(tmp);
		i++;
	}
	return (result);
}
