/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:59 by alberome          #+#    #+#             */
/*   Updated: 2026/03/07 21:10:13 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(const char *name)
{
	if (!name)
		return (false);
	return (ft_streq(name, "echo") || ft_streq(name, "cd")
		|| ft_streq(name, "pwd") || ft_streq(name, "export")
		|| ft_streq(name, "unset") || ft_streq(name, "env")
		|| ft_streq(name, "exit"));
}

int	exec_builtin(t_shell *sh, char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_streq(argv[0], "echo"))
		return (bi_echo(argv));
	if (ft_streq(argv[0], "cd"))
		return (bi_cd(sh, argv));
	if (ft_streq(argv[0], "pwd"))
		return (bi_pwd());
	if (ft_streq(argv[0], "export"))
		return (bi_export(sh, argv));
	if (ft_streq(argv[0], "unset"))
		return (bi_unset(sh, argv));
	if (ft_streq(argv[0], "env"))
		return (bi_env(sh));
	if (ft_streq(argv[0], "exit"))
		return (bi_exit(sh, argv));
	return (1);
}
