/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:40:58 by mafarino          #+#    #+#             */
/*   Updated: 2025/06/22 16:34:57 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	return (0);
}

/*
int	main(void)
{
	char	*str;
	int		i;

	str = "bulbasaur";
	i = 'f';
	printf("%s\n", ft_strrchr(str, i));
	printf("%s\n", strrchr(str, i));
	return (0);
}
*/