/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:15:24 by makpolat          #+#    #+#             */
/*   Updated: 2025/07/28 16:57:33 by ysumeral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*my_strdup(char *src)
{
	char	*dst;
	int		len;
	int		i;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dst = malloc(len + 1);
	if (!dst)
	{
		printf("my_strdup: malloc failed!\n");
		return (NULL);
	}
	printf("my_strdup: allocated at %p\n", dst);
	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	printf("my_strdup: copied string '%s' to %p\n", dst, dst);
	return (dst);
}

static char	*expand_var(char *str, int *i, t_envp *env_list)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*result;
	int		j;

	start = ++(*i);
	while (str[*i] && is_var_char(str[*i]))
		(*i)++;
	if (start == *i)
		return (my_strdup("$"));
	var_name = malloc(*i - start + 1);
	if (!var_name)
		return (NULL);
	j = 0;
	while (start < *i)
		var_name[j++] = str[start++];
	var_name[j] = '\0';
	value = get_env_value(var_name, env_list);
	printf("Variable '%s' = '%s' (ptr: %p)\n", var_name, value, value);
	if (!value)
	{
		printf("get_env_value returned NULL\n");
		free(var_name);
		return (my_strdup(""));
	}
	result = my_strdup(value);
	printf("Duplicated result: %p\n", result);
	if (!result)
	{
		printf("my_strdup failed!\n");
		free(var_name);
		return (NULL);
	}
	free(var_name);
	return (result);
}

static void	handle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static void	append_expansion(char **result, int *j, char *str, int *i, t_envp *env_list, int buffer_size)
{
	char	*temp;
	int		len;
	int		k;

	temp = expand_var(str, i, env_list);
	if (temp)
	{
		len = ft_strlen(temp);		
		k = 0;
		
		// Güvenli kopyalama - buffer overflow'u önle
		while (k < len && *j < buffer_size - 1)
		{
			(*result)[*j] = temp[k];
			(*j)++;
			k++;
		}
		
		// Eğer tüm içerik kopyalanamadıysa uyar
		if (k < len)
		{
			printf("WARNING: Content truncated! Copied %d/%d chars, buffer full at %d/%d\n", 
				k, len, *j, buffer_size);
		}
		else
		{
			printf("Successfully copied %d chars from expansion\n", len);
		}
		
		free(temp);
		printf("Freed expansion temp successfully\n");
	}
	else
	{
		printf("expand_var returned NULL\n");
	}
}

static char	*process_string(char *str, t_envp *env_list)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;
	int		buffer_size;

	if (!str)
		return (NULL);
	
	// Dinamik buffer boyutu hesaplama - değişken genişleme boyutunu tahmin et
	int estimated_expansion_size = 0;
	int idx = 0;
	
	// String içindeki her $ değişkeni için tahmini boyut hesapla
	while (str[idx])
	{
		if (str[idx] == '$' && str[idx + 1] && is_var_char(str[idx + 1]))
		{
			// Değişken ismini bul
			int var_start = idx + 1;
			int var_end = var_start;
			while (str[var_end] && is_var_char(str[var_end]))
				var_end++;
			
			// Değişken ismini çıkar
			char *var_name = malloc(var_end - var_start + 1);
			if (var_name)
			{
				int j = 0;
				while (var_start < var_end)
					var_name[j++] = str[var_start++];
				var_name[j] = '\0';
				
				// Değişkenin değerini al ve boyutunu hesapla
				char *var_value = get_env_value(var_name, env_list);
				if (var_value && ft_strlen(var_value) > 0)
					estimated_expansion_size += ft_strlen(var_value);
				else
					estimated_expansion_size += 1; // Boş değişken için
				
				free(var_name);
			}
			idx = var_end;
		}
		else
		{
			estimated_expansion_size++; // Normal karakter
			idx++;
		}
	}
	
	// Buffer boyutunu hesapla
	buffer_size = estimated_expansion_size + 200; // Güvenlik marjı
	if (buffer_size < 500)
		buffer_size = 500;  // Minimum 500 byte
	if (buffer_size > 50000)
		buffer_size = 50000; // Maksimum 50KB
	result = malloc(buffer_size);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	if (!result)
		return (NULL);
	while (str[i])
	{
		// Buffer overflow kontrolü - güvenli marj bırak
		if (j >= buffer_size - 10)
		{
			printf("WARNING: Approaching buffer limit at j=%d/%d, stopping processing\n", j, buffer_size);
			break;
		}
		
		if ((str[i] == '\'' || str[i] == '"'))
		{
			handle_quotes(str[i], &in_single, &in_double);
			result[j++] = str[i++];
		}
		else if (str[i] == '$' && !in_single
			&& str[i + 1] && is_var_char(str[i + 1]))
			append_expansion(&result, &j, str, &i, env_list, buffer_size);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	has_expansion(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '$' && !in_single && str[i + 1]
			&& is_var_char(str[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

void	parse_dollar(t_command *head)
{
	t_command	*iter;
	char		*processed;
	int			i;

	iter = head;
	while (iter)
	{
		i = 0;
		while (iter->args && iter->args[i])
		{
			if (!iter->args[i])
			{
				i++;
				continue;
			}
			
			// Skip expansion eğer single quote içindeyse
			if (iter->skip_expansion && iter->skip_expansion[i])
			{
				printf("Skipping expansion for arg[%d]: '%s' (was in single quotes)\n", i, iter->args[i]);
				i++;
				continue;
			}
			
			iter->dollar = has_expansion(iter->args[i]);
			processed = process_string(iter->args[i], iter->env_list);
			if (processed)
			{
				free(iter->args[i]);
				iter->args[i] = processed;
			}
			i++;
		}
		iter = iter->next;
	}
	execute(head);
	print_command_list(head);
	memory_free(head);
}
