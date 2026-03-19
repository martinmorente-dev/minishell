/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:12 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:28:40 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_operator_token(char *line, int *i)
{
	t_token_type	type;
	char			op[3];
	int				len;

	type = TOKEN_WORD;
	len = 0;
	if (line[*i] == '|')
	{
		type = TOKEN_PIPE;
		op[len++] = '|';
	}
	else if (line[*i] == '<' || line[*i] == '>')
		len = detect_redir_type(line[*i], line[*i + 1], &type, op);
	else
		op[len++] = line[*i];
	*i += len;
	return (create_op_token(type, op, len));
}

char	*get_word(char *line, int *i)
{
	int		start;
	int		len;

	start = *i;
	while (line[*i] && !is_whitespace(line[*i])
		&& !is_operator(line[*i]) && !is_quote(line[*i]))
		(*i)++;
	len = *i - start;
	return (ft_substr(line, start, len));
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	int		i;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		i = skip_whitespace(line, i);
		if (!line[i])
			break ;
		if (is_operator(line[i]))
			handle_operator(line, &i, &tokens);
		else if (is_quote(line[i]))
			handle_quote(line, line[i], &i, &tokens);
		else
			add_word_token(line, &i, &tokens);
	}
	return (tokens);
}
