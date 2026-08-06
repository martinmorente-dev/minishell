/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:40:15 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 21:52:10 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_parenthesized(const char *s);
int		run_eval_segment(t_shell *sh, char *seg);

static void	skip_paren_block(const char *s, size_t *i, int *quote)
{
	size_t	depth;

	depth = 1;
	++(*i);
	while (s[*i] && depth)
	{
		if ((s[*i] == '"' || s[*i] == '\'') && !*quote)
			*quote = s[(*i)++];
		else if (*quote && s[*i] == *quote)
		{
			*quote = 0;
			++(*i);
		}
		else if (!*quote)
		{
			if (s[*i] == '(')
				++depth;
			else if (s[*i] == ')')
				--depth;
			++(*i);
		}
		else
			++(*i);
	}
}

static bool	set_logic_split(char **out_seg, const char *s,
		size_t start, size_t i)
{
	*out_seg = ft_strndup(s + start, i - start);
	return (true);
}

static void	advance_logic_scan(const char *s, size_t *i, int *quote)
{
	if (!*quote && s[*i] == '(')
	{
		skip_paren_block(s, i, quote);
		return ;
	}
	if (!*quote && (s[*i] == '"' || s[*i] == '\''))
		*quote = s[*i];
	else if (*quote && s[*i] == *quote)
		*quote = 0;
	++(*i);
}

static bool	split_next_logic(const char *s, size_t *pos,
		char **out_seg, int *op)
{
	size_t	i;
	size_t	start;
	int		quote;

	i = *pos;
	while (s[i] == ' ' || s[i] == '\t')
		++i;
	start = i;
	quote = 0;
	while (s[i])
	{
		if (!quote && s[i] == '&' && s[i + 1] == '&')
			return (*pos = i + 2, *op = 1,
				set_logic_split(out_seg, s, start, i));
		if (!quote && s[i] == '|' && s[i + 1] == '|')
			return (*pos = i + 2, *op = 2,
				set_logic_split(out_seg, s, start, i));
		advance_logic_scan(s, &i, &quote);
	}
	*out_seg = ft_strndup(s + start, i - start);
	*pos = i;
	*op = 0;
	return (true);
}

bool	eval_line(t_shell *sh, const char *line, int *out_status)
{
	size_t	pos;
	int		st;
	int		op;
	char	*segment;
	int		next;

	pos = 0;
	st = 0;
	op = 0;
	while (line[pos])
	{
		segment = NULL;
		next = 0;
		if (!split_next_logic(line, &pos, &segment, &next))
			return (false);
		if (op == 0 || (op == 1 && st == 0) || (op == 2 && st != 0))
			st = run_eval_segment(sh, trim_segment(segment));
		free(segment);
		if (next == 0)
			break ;
		op = next;
	}
	if (out_status)
		*out_status = st;
	return (true);
}
