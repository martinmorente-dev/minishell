/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:22:50 by alberome          #+#    #+#             */
/*   Updated: 2024/12/17 20:33:11 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t		inicio;
	size_t		fin;
	char		*dest;

	inicio = 0;
	fin = ft_strlen(s1) - 1;
	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (s1[inicio] && ft_strchr(set, s1[inicio]))
		++inicio;
	while (s1[fin] && ft_strchr(set, s1[fin]))
		--fin;
	dest = ft_substr(s1, inicio, fin - inicio + 1);
	return (dest);
}
