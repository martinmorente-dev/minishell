/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:39:10 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 21:27:18 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_chunk(char *out, size_t *out_len,
		const char *chunk, size_t chunk_len)
{
	char	*new_out;

	new_out = (char *)malloc(*out_len + chunk_len + 1);
	if (!new_out)
	{
		free(out);
		ft_malloc_error();
		return (NULL);
	}
	if (*out_len > 0)
		ft_memcpy(new_out, out, *out_len);
	if (chunk_len > 0)
		ft_memcpy(new_out + *out_len, chunk, chunk_len);
	new_out[*out_len + chunk_len] = '\0';
	free(out);
	*out_len += chunk_len;
	return (new_out);
}

static char	*read_single_quoted(const char **s)
{
	const char	*p;
	size_t		len;

	p = ++(*s);
	while (**s && **s != '\'')
		++(*s);
	len = (size_t)(*s - p);
	if (**s == '\'')
		++(*s);
	return (ft_strndup(p, len));
}

static char	*read_double_quoted(t_shell *sh, const char **s)
{
	const char	*p;
	size_t		len;
	char		*raw;
	char		*expanded;

	++(*s);
	p = *s;
	while (**s && **s != '"')
		++(*s);
	len = (size_t)(*s - p);
	if (**s == '"')
		++(*s);
	raw = ft_strndup(p, len);
	if (!raw)
		return (NULL);
	expanded = expand_vars_dquote(sh, raw);
	free(raw);
	return (expanded);
}

static char	*read_chunk(t_shell *sh, const char **s)
{
	const char	*p;
	char		*raw;
	char		*chunk;

	if (**s == '\'')
		return (read_single_quoted(s));
	if (**s == '"')
		return (read_double_quoted(sh, s));
	p = *s;
	while (**s && **s != ' ' && **s != '\t' && **s != '\''
		&& **s != '"' && **s != '|' && **s != '<' && **s != '>')
		++(*s);
	if (*s <= p)
		return (NULL);
	raw = ft_strndup(p, (size_t)(*s - p));
	if (!raw)
		return (NULL);
	chunk = expand_vars(sh, raw);
	free(raw);
	return (chunk);
}

char	*read_word(t_shell *sh, const char **s)
{
	char	*out;
	size_t	out_len;
	char	*chunk;

	out = NULL;
	out_len = 0;
	while (**s && **s != ' ' && **s != '\t' && **s != '|'
		&& **s != '<' && **s != '>')
	{
		chunk = read_chunk(sh, s);
		if (!chunk)
			return (free(out), NULL);
		out = append_chunk(out, &out_len, chunk, ft_strlen(chunk));
		free(chunk);
		if (!out)
			return (NULL);
	}
	if (out)
		return (out);
	return (ft_strndup("", 0));
}
