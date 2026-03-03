/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:24:00 by mafarino          #+#    #+#             */
/*   Updated: 2024/06/23 18:46:56 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest == src || !n)
		return (dest);
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	while (n-- > 0)
	{
		*((unsigned char *)dest + n) = *((unsigned char *)src + n);
	}
	return (dest);
}
