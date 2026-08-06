/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:38:41 by alberome          #+#    #+#             */
/*   Updated: 2026/03/08 18:39:16 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir_op(const char *tok)
{
	return (ft_streq(tok, "<") || ft_streq(tok, ">")
		|| ft_streq(tok, ">>") || ft_streq(tok, "<<"));
}

t_redir_type	redir_type_for(const char *op)
{
	if (ft_streq(op, "<"))
		return (REDIR_IN);
	if (ft_streq(op, ">>"))
		return (REDIR_APPEND);
	if (ft_streq(op, "<<"))
		return (REDIR_HEREDOC);
	return (REDIR_OUT);
}

static size_t	count_redirs_in_segment(char **toks, size_t start, size_t end)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = start;
	while (i < end)
	{
		if (!ft_streq(toks[i], "|") && is_redir_op(toks[i]))
		{
			++count;
			++i;
		}
		++i;
	}
	return (count);
}

static void	fill_one_redir(char **toks, size_t *i,
		size_t end, t_redir *redir)
{
	redir->type = redir_type_for(toks[*i]);
	redir->quoted_delim = false;
	++(*i);
	if (*i < end && toks[*i] != NULL)
	{
		redir->arg = ft_strdup(toks[*i]);
		++(*i);
	}
	else
		redir->arg = ft_strdup("");
}

bool	build_redirs(char **toks, size_t start, size_t end, t_cmd *cmd)
{
	size_t	rcount;
	size_t	i;
	size_t	ri;

	rcount = count_redirs_in_segment(toks, start, end);
	if (rcount == 0)
	{
		cmd->redirs = NULL;
		cmd->redirs_count = 0;
		return (true);
	}
	cmd->redirs = (t_redir *)ft_calloc(rcount, sizeof(t_redir));
	if (!cmd->redirs)
		return (false);
	cmd->redirs_count = rcount;
	ri = 0;
	i = start;
	while (i < end)
	{
		if (!ft_streq(toks[i], "|") && is_redir_op(toks[i]))
			fill_one_redir(toks, &i, end, &cmd->redirs[ri++]);
		else
			++i;
	}
	return (true);
}
