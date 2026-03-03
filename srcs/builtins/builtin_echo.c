/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:16:28 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/10 16:56:31 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Builtin echo
 *
 * PARAMS:
 * 
 * input: The input that the user wants to print
 * 
 *
 * RETURN:
 *
 * Return a int indicating 0 = success 1 = error
*/
int	builtin_echo(char **input)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	while (input[i] && ft_strncmp(input[i], "-n", 2) == 0)
	{
		n_flag = true;
		i++;
	}
	while (input[i])
	{
		ft_putstr_fd(input[i], STDOUT_FILENO);
		if (input[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
