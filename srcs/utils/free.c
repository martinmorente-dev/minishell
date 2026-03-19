/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:07:41 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 19:31:23 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_double_ptr(void **ptr_array)
{
	int	i;

	if (!ptr_array)
		return ;
	i = 0;
	while (ptr_array[i])
	{
		free(ptr_array[i]);
		i++;
	}
	free(ptr_array);
}

void	cleanup_shell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->line)
		free(shell->line);
	if (shell->commands)
		free_commands(shell->commands);
	free(shell);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;
	int		i;

	while (cmds)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		i = 0;
		while (tmp_cmd->args && tmp_cmd->args[i])
			free(tmp_cmd->args[i++]);
		free(tmp_cmd->args);
		while (tmp_cmd->redirs)
		{
			tmp_redir = tmp_cmd->redirs;
			tmp_cmd->redirs = tmp_cmd->redirs->next;
			free(tmp_redir->file);
			free(tmp_redir);
		}
		free(tmp_cmd);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
