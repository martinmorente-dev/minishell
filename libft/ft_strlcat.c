/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 20:45:30 by mafarino          #+#    #+#             */
/*   Updated: 2024/06/23 18:47:41 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	size_dst;
	size_t	size_src;

	size_dst = ft_strlen(dst);
	size_src = ft_strlen(src);
	i = size_dst;
	if (size <= size_dst)
		return (size + size_src);
	while (*src != '\0' && i < (size - 1))
	{
		*(dst + i++) = *src++;
	}
	*(dst + i) = '\0';
	return (size_dst + size_src);
}
/*int main(void)
{
	char dest[10] = "789";
	char src[] = "123456";
	size_t i = ft_strlcat(dest, src, 8);
	printf("now dest is %s, and result is %zu", dest, i);
}*/