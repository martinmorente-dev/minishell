/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:37:41 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 18:40:51 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_copy_strings(char *dest, const char *a,
			const char *b, const char *c)
{
	size_t	k;
	size_t	i;

	k = 0;
	i = 0;
	while (i < ft_strlen(a))
	{
		dest[k++] = a[i];
		++i;
	}
	i = 0;
	while (i < ft_strlen(b))
	{
		dest[k++] = b[i];
		++i;
	}
	i = 0;
	while (i < ft_strlen(c))
	{
		dest[k++] = c[i];
		++i;
	}
	dest[k] = '\0';
}

char	*ft_strjoin3(const char *a, const char *b, const char *c)
{
	size_t	na;
	size_t	nb;
	size_t	nc;
	char	*p;

	na = ft_strlen(a);
	nb = ft_strlen(b);
	nc = ft_strlen(c);
	p = (char *)malloc(na + nb + nc + 1);
	if (!p)
	{
		ft_malloc_error();
		return (NULL);
	}
	ft_copy_strings(p, a, b, c);
	return (p);
}

char	**ft_strarray_dup(char **arr)
{
	size_t	n;
	char	**out;
	size_t	i;

	if (!arr)
		return (NULL);
	n = ft_strarray_len(arr);
	out = (char **)ft_calloc(n + 1, sizeof(char *));
	if (!out)
		return (NULL);
	i = 0;
	while (i < n)
	{
		out[i] = ft_strdup(arr[i]);
		if (!out[i])
		{
			ft_strarray_free(out);
			return (NULL);
		}
		++i;
	}
	out[n] = NULL;
	return (out);
}

void	ft_strarray_free(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		++i;
	}
	free(arr);
}

size_t	ft_strarray_len(char **arr)
{
	size_t	n;

	n = 0;
	if (!arr)
		return (0);
	while (arr[n])
		++n;
	return (n);
}
