/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:17:24 by alberome          #+#    #+#             */
/*   Updated: 2024/12/15 16:55:03 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(int c)
{
	if ((65 <= c) && (c <= 90))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

static int	ft_islower(int c)
{
	if ((97 <= c) && (c <= 122))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	ft_isalpha(int c)
{
	return (ft_isupper(c) || ft_islower(c));
}
