/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:38:56 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 21:22:20 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**resize_tokens_array(char **tokens, size_t len, size_t *cap)
{
	size_t	new_cap;
	char	**new_tokens;
	size_t	i;

	new_cap = (*cap) * 2;
	new_tokens = (char **)ft_calloc(new_cap, sizeof(char *));
	if (!new_tokens)
	{
		ft_strarray_free(tokens);
		ft_malloc_error();
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_tokens[i] = tokens[i];
		++i;
	}
	free(tokens);
	*cap = new_cap;
	return (new_tokens);
}

static int	add_token(char ***tokens, size_t *len, size_t *cap, char *tok)
{
	if (*len + 1 >= *cap)
	{
		*tokens = resize_tokens_array(*tokens, *len, cap);
		if (!*tokens)
		{
			free(tok);
			return (0);
		}
	}
	(*tokens)[(*len)++] = tok;
	(*tokens)[*len] = NULL;
	return (1);
}

static char	*read_redir_op(const char **s)
{
	char	op;
	char	buf[3];

	op = **s;
	buf[0] = op;
	buf[1] = '\0';
	buf[2] = '\0';
	++(*s);
	if (**s == op && (op == '<' || op == '>'))
	{
		buf[1] = op;
		buf[2] = '\0';
		++(*s);
	}
	return (ft_strdup(buf));
}

static int	lex_one_token(t_shell *sh, const char **s,
		char ***tokens, size_t tl[2])
{
	char	*tok;

	if (**s == '|')
	{
		tok = ft_strdup("|");
		if (!tok || !add_token(tokens, &tl[0], &tl[1], tok))
			return (-1);
		++(*s);
		return (1);
	}
	if (**s == '<' || **s == '>')
	{
		tok = read_redir_op(s);
		if (!tok || !add_token(tokens, &tl[0], &tl[1], tok))
			return (-1);
		return (1);
	}
	tok = read_word(sh, s);
	if (!tok)
		return (-1);
	if (!tok[0])
		free(tok);
	else if (!add_token(tokens, &tl[0], &tl[1], tok))
		return (-1);
	return (1);
}

char	**simple_lex(t_shell *sh, const char *line)
{
	const char	*s;
	char		**tokens;
	size_t		tl[2];

	s = line;
	tl[0] = 0;
	tl[1] = 16;
	tokens = (char **)ft_calloc(tl[1], sizeof(char *));
	if (!tokens)
		return (NULL);
	while (*s)
	{
		while (*s == ' ' || *s == '\t')
			++s;
		if (!*s)
			break ;
		if (lex_one_token(sh, &s, &tokens, tl) < 0)
		{
			ft_strarray_free(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
