/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:29:42 by alberome          #+#    #+#             */
/*   Updated: 2025/05/22 18:21:15 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_help(va_list arg, const char letter)
{
	if (letter == 'c')
		return (ft_putchar(va_arg(arg, int)));
	else if (letter == 's')
		return (ft_putstr(va_arg(arg, char *)));
	else if (letter == 'p')
		return (ft_print_ptr(va_arg(arg, unsigned long long)));
	else if (letter == 'd' || letter == 'i')
		return (ft_putnbr(va_arg(arg, int)));
	else if (letter == 'u')
		return (ft_print_unsigned(va_arg(arg, int)));
	else if (letter == 'x')
		return (ft_print_hex(va_arg(arg, int), "0123456789abcdef"));
	else if (letter == 'X')
		return (ft_print_hex(va_arg(arg, int), "0123456789ABCDEF"));
	else if (letter == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(char const *str, ...)
{
	va_list	args;
	int		i;
	int		length;

	i = 0;
	length = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			length += ft_help(args, str[i + 1]);
			i++;
		}
		else
		{
			length += ft_putchar(str[i]);
		}
		i++;
	}
	va_end(args);
	return (length);
}
