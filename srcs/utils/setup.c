/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 18:28:19 by mmorente          #+#    #+#             */
/*   Updated: 2026/03/03 18:40:08 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	shell->env = init_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->commands = NULL;
	shell->line = NULL;
	shell->exit_code = 0;
	shell->exit_flag = 0;
	return (shell);
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
