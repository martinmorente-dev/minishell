/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:10 by alberome          #+#    #+#             */
/*   Updated: 2026/03/07 21:09:38 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_n_flag(const char *s)
{
	if (!s || s[0] != '-' || s[1] != 'n')
		return (false);
	s += 2;
	while (*s)
	{
		if (*s != 'n')
			return (false);
		++s;
	}
	return (true);
}

int	bi_echo(char **argv)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = false;
		++i;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		++i;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
