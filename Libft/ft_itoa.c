/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:13:35 by makpolat          #+#    #+#             */
/*   Updated: 2024/11/05 12:17:54 by makpolat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
	{
		count++;
	}
	while (n)
	{
		n /= 10;
		++count;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int			len;
	char		*str;
	const char	*digits;

	digits = "0123456789";
	len = ft_count(n);
	str = ft_calloc(len + 1, sizeof(char));
	if (str == 0)
		return (0);
	if (n == 0)
		str[0] = '0';
	if (n < 0)
		str[0] = '-';
	while (n)
	{
		if (n > 0)
			str[--len] = digits[n % 10];
		else
			str[--len] = digits[n % 10 * -1];
		n /= 10;
	}
	return (str);
}
