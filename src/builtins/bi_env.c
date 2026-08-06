/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:26 by alberome          #+#    #+#             */
/*   Updated: 2026/03/07 21:09:48 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_env(t_shell *sh)
{
	t_env	*e;

	e = sh->env;
	while (e)
	{
		if (e->value)
		{
			write(STDOUT_FILENO, e->key, ft_strlen(e->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, e->value, ft_strlen(e->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		e = e->next;
	}
	return (0);
}
