/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:36:12 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 17:48:36 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_shlvl(t_shell *sh)
{
	char	*cur;
	long	lvl;
	char	*lvl_str;

	cur = env_get(sh->env, "SHLVL");
	lvl = 0;
	if (cur)
		lvl = ft_atol(cur);
	lvl_str = ft_itoa(lvl + 1);
	if (lvl_str)
	{
		env_set(&sh->env, "SHLVL", lvl_str, true);
		free(lvl_str);
	}
}

int	shell_loop(t_shell *sh)
{
	char	*line;
	int		status;

	update_shlvl(sh);
	while (sh->running)
	{
		line = readline("turtleshell$ ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		status = 0;
		sh->in_pipe = false;
		if (eval_line(sh, line, &status))
			sh->last_status = status;
		free(line);
	}
	return (sh->last_status);
}
