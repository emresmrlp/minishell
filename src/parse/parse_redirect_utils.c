/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:09:05 by makpolat          #+#    #+#             */
/*   Updated: 2025/08/03 14:30:26 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_to_fd_array(char ***fd_array, char *filename)
{
	int		count;
	char	**new_array;
	int		i;

	count = 0;
	if (*fd_array)
	{
		while ((*fd_array)[count])
			count++;
	}
	new_array = malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return ;
	i = 0;
	while (i < count)
	{
		new_array[i] = (*fd_array)[i];
		i++;
	}
	new_array[count] = ft_strdup(filename);
	new_array[count + 1] = NULL;
	if (*fd_array)
		free(*fd_array);
	*fd_array = new_array;
}

int	has_single_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	calculate_buffer_size(char *str, char delimiter)
{
	int	estimated_tokens;
	int	buffer_size;
	int	i;

	estimated_tokens = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
			estimated_tokens++;
		i++;
	}
	buffer_size = estimated_tokens + 10;
	if (buffer_size < 20)
		buffer_size = 20;
	if (buffer_size > 1000)
		buffer_size = 1000;
	return (buffer_size);
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
