/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:16:54 by alberome          #+#    #+#             */
/*   Updated: 2024/12/17 20:12:03 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(long int n)
{
	int	contador;

	contador = 0;
	if (n <= 0)
	{
		contador++;
		if (n == 0)
			return (contador);
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		contador++;
	}
	return (contador);
}

static char	*ft_fill(int len, long int n, char *cadena)
{
	while (n > 0)
	{
		cadena[len - 1] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	return (cadena);
}

char	*ft_itoa(int n)
{
	long int	len;
	char		*cadena;
	long int	m;

	m = n;
	len = ft_count((long)n);
	cadena = (char *)malloc(sizeof(char) * len + 1);
	if (!cadena)
		return (NULL);
	cadena[len] = '\0';
	if (m < 0)
	{
		cadena[0] = '-';
		m = -m;
	}
	if (m == 0)
	{
		cadena[0] = '0';
		cadena[1] = '\0';
		return (cadena);
	}
	ft_fill(len, m, cadena);
	return (cadena);
}
