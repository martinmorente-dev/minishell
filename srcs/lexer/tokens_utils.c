/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:17 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:00:37 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_whitespace(char *line, int i)
{
	while (is_whitespace(line[i]))
		i++;
	return (i);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t'
		| c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

void	handle_operator(char *line, int *i, t_token **tokens)
{
	t_token	*token;

	token = get_operator_token(line, i);
	if (token)
		add_token(tokens, token);
	else
		free_tokens(*tokens);
}

void	print_tokens(t_token *tokens)
{
	const char	*type_names[] = {
		"TOKEN_WORD", "TOKEN_PIPE", "TOKEN_REDIR_IN", "TOKEN_REDIR_OUT",
		"TOKEN_REDIR_APPEND", "TOKEN_HEREDOC", "TOKEN_ENV_VAR",
		"TOKEN_SQUOTE", "TOKEN_DQUOTE"
	};

	while (tokens)
	{
		printf("[%s] \"%s\"\n", type_names[tokens->type], tokens->value);
		tokens = tokens->next;
	}
}

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}
