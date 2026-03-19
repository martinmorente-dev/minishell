/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:36 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:18:45 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		count;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (false);
	count = 0;
	while (cmd->args && cmd->args[count])
	{
		new_args[count] = cmd->args[count];
		count++;
	}
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
	{
		free(new_args);
		return (false);
	}
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (true);
}

bool	add_redir(t_cmd *cmd, t_redir_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (false);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	if (!new_redir->file)
	{
		free(new_redir);
		return (false);
	}
	new_redir->fd = -1;
	new_redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (true);
}

t_cmd	*append_cmd(t_cmd *cmds, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!cmds)
		return (new_cmd);
	current = cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
	return (cmds);
}

void	skip_quote(char *quote, int *i)
{
	if (!*quote)
		*quote = (*i)++;
	else
	{
		*quote = 0;
		(*i)++;
	}
}

void	handle_pipe(t_cmd **cur_cmd, t_cmd *cmds)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd();
	if (!new_cmd)
	{
		free_commands(cmds);
		*cur_cmd = NULL;
		return ;
	}
	cmds = append_cmd(cmds, new_cmd);
	*cur_cmd = new_cmd;
}
