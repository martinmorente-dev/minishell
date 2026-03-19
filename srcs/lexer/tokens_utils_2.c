/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:17 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:35:49 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token	*create_op_token(t_token_type type, char *op, int len)
{
	op[len] = '\0';
	return (create_token(type, op));
}

bool	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	add_word_token(char *line, int *i, t_token **tokens)
{
	char	*word;
	t_token	*token;
	int		start;

	start = *i;
	while (line[*i] && !is_whitespace(line[*i])
		&& !is_operator(line[*i]) && !is_quote(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (!word)
	{
		free_tokens(*tokens);
		return ;
	}
	token = create_token(TOKEN_WORD, word);
	free(word);
	if (!token)
		free_tokens(*tokens);
	else
		add_token(tokens, token);
}
