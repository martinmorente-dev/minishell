/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 18:50:05 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/10 16:57:02 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Builtin pwd
 *
 * FUN:
 *
 * Prints the absolute route of the
 * working directory of the process that was callen
 *
 * PARAMS:
 * 
 * no params
 * 
 *
 * RETURN:
 *
 * Return a int indicating 0 = success 1 = error
*/
int	builtin_pwd(void)
{
	ft_putendl_fd(getcwd(NULL, 0), STDOUT_FILENO);
	return (0);
}
