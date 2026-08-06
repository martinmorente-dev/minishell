/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:37:55 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 18:40:17 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_perror(const char *prefix)
{
	if (prefix)
	{
		write(STDERR_FILENO, prefix, ft_strlen(prefix));
		write(STDERR_FILENO, ": ", 2);
	}
	perror(NULL);
}

void	ft_print_error3(const char *a, const char *b, const char *c)
{
	if (a)
		write(STDERR_FILENO, a, ft_strlen(a));
	if (b)
		write(STDERR_FILENO, b, ft_strlen(b));
	if (c)
		write(STDERR_FILENO, c, ft_strlen(c));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_malloc_error(void)
{
	write(STDERR_FILENO, "minishell: malloc error\n", 24);
	exit(EXIT_FAILURE);
}

char	*ft_getcwd_dup(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}
