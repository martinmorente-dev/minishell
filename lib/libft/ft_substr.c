/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:50:17 by alberome          #+#    #+#             */
/*   Updated: 2024/12/16 21:51:50 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*cadena;
	size_t	numero;

	numero = 0;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (!s || start < 0 || len < 0)
		return (NULL);
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	cadena = (char *)malloc((len + 1) * sizeof(char));
	if (cadena == NULL)
		return (NULL);
	while (len-- > 0 && s[start] != '\0')
	{
		cadena[numero++] = s[start];
		start++;
	}
	cadena[numero] = '\0';
	return (cadena);
}
