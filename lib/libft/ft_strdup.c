/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:46:02 by alberome          #+#    #+#             */
/*   Updated: 2024/12/15 15:44:27 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	len;

	len = ft_strlen((char *)s) + 1;
	dest = malloc(len);
	if (dest == NULL)
	{
		return (NULL);
	}
	ft_memcpy(dest, (void *)s, len);
	return (dest);
}
