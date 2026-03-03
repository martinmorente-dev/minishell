/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 20:44:36 by mafarino          #+#    #+#             */
/*   Updated: 2024/06/23 17:57:26 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//#include < stdio.h >  // Для printf
//#include < string.h > // Для strcpy
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = ft_strlen(src);
	if (size == 0)
		return (i);
	while (*src && size > 1)
	{
		*dst++ = *src++;
		size--;
	}
	*dst = '\0';
	return (i);
}

/*
int main (void)
{    

   char src[1024]="first str \0second str";
 
   char dst[1024]="";
   int i = ft_strcpy (dst, src, 5);

   printf ("src: %s %s\n",src, &src[14]);

   printf ("dst: %s %s\n",dst, &dst[14]);

   return 0;
}*/