
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:37:34 by mafarino          #+#    #+#             */
/*   Updated: 2024/06/23 18:56:47 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *str)
{
	long	a;
	long	b;
	long	i;

	a = 0;
	b = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			b = b * -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		a = a * 10 + str[i] - '0';
		i++;
	}
	return (a * b);
}

/*int	main(void)
{
	int	outcome;
	char	str[] = "1234";

	outcome = ft_atoi(str);
	printf("%d\n", outcome);
	outcome = atoi(str);
	printf("%d\n", outcome);
	return (0);
}*/
