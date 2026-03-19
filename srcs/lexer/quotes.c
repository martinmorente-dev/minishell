/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:01 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:35:28 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}


char	*get_quoted_word(char *line, int *i, char quote)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	(*i)++;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i] != quote)
		return (NULL);
	(*i)++;
	len = *i - start;
	word = ft_substr(line, start, len);

	return (word);
}

void	handle_quote(char *line, char c, int *i, t_token **tokens)
{
	char	*word;
	t_token	*token;

	word = get_quoted_word(line, i, c);
	if (word)
	{
		token = create_token(TOKEN_WORD, word);
		free(word);
		if (token)
			add_token(tokens, token);
		else
			free_tokens(*tokens);
	}
	else
		free_tokens(*tokens);
}
