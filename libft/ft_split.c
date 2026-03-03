/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marina <marina@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:26:50 by mafarino          #+#    #+#             */
/*   Updated: 2025/06/23 11:09:31 by marina           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
}

static int	ft_get_rows(char const *s, char c)
{
	int	rows;
	int	i;

	if(!s)
		return (0);
	rows = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			rows++;
		while (s[i] && s[i] != c)
				i++;
	}
	return (rows);
}

char	**ft_split(char const *s, char c)
{
	char	**new;
	char	*wordstart;
	int		i;
	int		rows;

	if (!s)
		return NULL;
	rows = ft_get_rows(s, c);
	if (rows == 0)
		return (NULL);
	new = (char **)malloc(sizeof(char *) * (rows + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			wordstart = (char *)s;
			while (*s != c && *s)
				s++;
			new[i] = ft_substr(wordstart, 0, s - wordstart);
			if (!new[i])
				return (ft_free(new));
			i++;
		}
		else
			s++;
	}
	new[i] = NULL;
	return (new);
}

/*int	main(void)
{
	char **array;
	char const s[] = "Bulbasaur is the best";
	char c = ' ';
	int	i;
	array = ft_split(s, c);
	printf("%d\n", ft_get_rows(s, c));
	//printf("%c", **ft_split(s, c));
	i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}*/
