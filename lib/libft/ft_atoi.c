/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:14:58 by alberome          #+#    #+#             */
/*   Updated: 2024/12/15 21:05:33 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_clear(char *s)
{
	int	i;

	i = 0;
	while (s[i] == '\t' || s[i] == '\n' || s[i] == ' '
		|| s[i] == '\r' || s[i] == '\f' || s[i] == '\v')
		i++;
	return (i);
}

int	ft_atoi(const char *s)
{
	int	i;
	int	n;
	int	sign;
	int	cuenta;

	cuenta = 0;
	i = ft_clear((char *)s);
	n = 0;
	sign = 1;
	while (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		cuenta++;
		i++;
	}
	if (cuenta > 1)
		return (0);
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = (10 * n) + (s[i] - '0');
		i++;
	}
	return (n * sign);
}
