/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:24:57 by mafarino          #+#    #+#             */
/*   Updated: 2024/06/23 19:14:30 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*d;
	unsigned const char	*s;

	d = (unsigned char *) s1;
	s = (unsigned char *) s2;
	if (n)
	{
		while (n--)
		{
			if (*d++ != *s++)
				return (*(--d) - *(--s));
		}
	}
	return (0);
}
