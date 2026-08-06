/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 20:54:23 by alberome          #+#    #+#             */
/*   Updated: 2024/12/15 19:40:18 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;
	unsigned int	result;
	unsigned int	find;
	size_t			len;

	len = ft_strlen((char *)s);
	i = 0;
	result = 0;
	find = 0;
	if ((unsigned char)c == '\0')
	{
		return (&((char *)s)[len]);
	}
	while (s[i])
	{
		if ((unsigned char)c == s[i])
		{
			result = i;
			find++;
		}
		i++;
	}
	if (find == 0)
		return (0);
	return (&((char *)s)[result]);
}
