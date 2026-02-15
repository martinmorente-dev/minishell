/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martin <martin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 19:02:28 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/15 19:46:53 by martin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_minishell *shell, char ** args)
{
	if (args[1])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	shell->exit_flag = 1;
	return (0);
}
