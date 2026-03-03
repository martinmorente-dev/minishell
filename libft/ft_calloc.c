
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:25:22 by mafarino          #+#    #+#             */
/*   Updated: 2025/06/22 17:12:13 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	i;

	i = 0;
	ptr = malloc (count * size);
	if (ptr == NULL)
		return (NULL);
	while (i < (count * size))
	{
		(*(unsigned char *)(ptr + i)) = 0;
			i++;
	}
	return (ptr);
}

/*void	*ft_calloc(size_t count, size_t size);

int	main(void)
{
	char	*str = ft_calloc (3, sizeof(int));
	unsigned int	i;

	i = 0;
	while (i < 12)
	{
		printf("%c", str[i]);
		i++;
	}
	free (str);
	return (0);
}
*/