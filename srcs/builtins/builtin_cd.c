/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:57:19 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/10 18:50:43 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **input)
{
	struct stat	st;

	if (stat(input[1], &st) != 0)
	{
		perror("cd");
		return (1);
	}
	if (S_ISREG(st.st_mode))
	{
		errno = ENOTDIR;
		perror("cd");
		return (1);
	}
	chdir(input[1]);
	return (0);
}
