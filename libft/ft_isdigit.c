/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:06:36 by mafarino          #+#    #+#             */
/*   Updated: 2025/06/22 17:11:59 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/*int	ft_isdigit(int c);

int	main(void)
{
	int i;

	i = 'A';
	printf("%d\n", ft_isdigit(i));
	printf("%d\n", isdigit(i));
	return (0);
}*/