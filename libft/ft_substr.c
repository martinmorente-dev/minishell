/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marina <marina@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:26:09 by mafarino          #+#    #+#             */
/*   Updated: 2025/06/23 10:32:32 by marina           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		j;
	size_t		i;
	size_t		size;
	char		*str;

	size = ft_strlen(s);
	//if (!s || stary >= size)
	//	return strdup("");
	if (size <= len)
		len = size;
	str = (char *)malloc(sizeof(*s) * (len + 1));
	if (str == 0)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (i >= start && j < len)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}
/*int	main(void)
{
	char const s[] = "hello world";
	unsigned int	start = 7;
	size_t	len = 2;
	printf("%s", ft_substr(s,start,len));
	return (0);
}*/
