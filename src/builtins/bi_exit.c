/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:33 by alberome          #+#    #+#             */
/*   Updated: 2026/03/08 18:16:48 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_sign(const char **s)
{
	int	sign;

	sign = 1;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		++(*s);
	}
	return (sign);
}

static bool	ft_atol_safe(const char *s, long *out)
{
	long	result;
	int		sign;
	long	prev;

	if (!s || !*s)
		return (false);
	sign = get_sign(&s);
	if (!*s)
		return (false);
	result = 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (false);
		prev = result;
		result = result * 10 + (*s - '0');
		if (result < prev)
			return (false);
		++s;
	}
	*out = result * sign;
	return (true);
}

int	bi_exit(t_shell *sh, char **argv)
{
	int		status;
	long	lval;

	status = sh->last_status;
	if (!sh->in_pipe && isatty(STDIN_FILENO))
		write(STDERR_FILENO, "exit\n", 5);
	if (argv[1])
	{
		if (!ft_atol_safe(argv[1], &lval))
		{
			ft_print_error3("exit: ", argv[1], ": numeric argument required");
			sh->running = false;
			sh->last_status = 2;
			return (2);
		}
		if (argv[2])
		{
			ft_print_error3("exit: ", NULL, "too many arguments");
			return (1);
		}
		status = (int)(lval & 0xFF);
	}
	sh->running = false;
	sh->last_status = status;
	return (status);
}
