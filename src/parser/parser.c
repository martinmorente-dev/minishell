/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:38:21 by alberome          #+#    #+#             */
/*   Updated: 2026/03/26 18:38:48 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_segments(char **toks)
{
	size_t	n;
	size_t	i;

	n = 1;
	i = 0;
	while (toks[i])
		n += (size_t)(ft_streq(toks[i++], "|"));
	return (n);
}

static bool	emit_segment(char **toks, size_t start, size_t end, t_cmd *cmd)
{
	cmd->argv = slice_argv(toks, start, end);
	if (!build_redirs(toks, start, end, cmd))
		return (false);
	return (true);
}

static bool	build_pipeline_from_tokens(char **toks, t_pipeline *out)
{
	size_t	seg_idx;
	size_t	start;
	size_t	i;

	seg_idx = 0;
	start = 0;
	i = 0;
	out->cmds_count = count_segments(toks);
	out->cmds = (t_cmd *)ft_calloc(out->cmds_count, sizeof(t_cmd));
	if (!out->cmds)
		return (false);
	while (1)
	{
		if (!toks[i] || ft_streq(toks[i], "|"))
		{
			if (!emit_segment(toks, start, i, &out->cmds[seg_idx]))
				return (false);
			++seg_idx;
			if (!toks[i])
				break ;
			start = i + 1;
		}
		++i;
	}
	return (true);
}

bool	parse_line(t_shell *sh, const char *line, t_pipeline *out)
{
	char	**tokens;
	bool	ok;
	size_t	len;
	size_t	i;

	if (!line || !out)
		return (false);
	tokens = simple_lex(sh, line);
	if (!tokens)
		return (false);
	len = ft_strarray_len(tokens);
	if (len == 0 || ft_streq(tokens[0], "|") || ft_streq(tokens[len - 1], "|"))
	{
		ft_strarray_free(tokens);
		return (false);
	}
	i = 1;
	while (i + 1 < len)
		if (ft_streq(tokens[i++], "|") && ft_streq(tokens[i], "|"))
			return (ft_strarray_free(tokens), false);
	ok = build_pipeline_from_tokens(tokens, out);
	ft_strarray_free(tokens);
	return (ok);
}

void	free_pipeline(t_pipeline *p)
{
	size_t	i;
	size_t	j;

	if (!p || !p->cmds)
		return ;
	i = 0;
	while (i < p->cmds_count)
	{
		ft_strarray_free(p->cmds[i].argv);
		if (p->cmds[i].redirs)
		{
			j = 0;
			while (j < p->cmds[i].redirs_count)
			{
				free(p->cmds[i].redirs[j].arg);
				++j;
			}
			free(p->cmds[i].redirs);
		}
		++i;
	}
	free(p->cmds);
	p->cmds = NULL;
	p->cmds_count = 0;
}
