/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:12 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/08 15:41:57 by mafarino         ###   ########.fr       */
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
		return;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}


static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}


static bool	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static t_token	*get_operator_token(char *line, int *i)
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
	else if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			type = TOKEN_HEREDOC;
			op[len++] = '<';
			op[len++] = '<';
		}
		else
		{
			type = TOKEN_REDIR_IN;
			op[len++] = '<';
		}
	}
	else if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			type = TOKEN_REDIR_APPEND;
			op[len++] = '>';
			op[len++] = '>';
		}
		else
		{
			type = TOKEN_REDIR_OUT;
			op[len++] = '>';
		}
	}
	else
    {
        op[len++] = line[*i];
    }
	op[len] = '\0';
	*i += len;
	return (create_token(type, op));
}

static char	*get_quoted_word(char *line, int *i, char quote)
{
	int		start;
	int		len;
	char	*word;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i] != quote)
		return (NULL);
	len = *i - start;
	word = ft_substr(line, start, len);
	(*i)++;
	return (word);
}

static char	*get_word(char *line, int *i)
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
	t_token	*new_token;
	char	*word;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (is_whitespace(line[i]))
			i++;
		if (!line[i])
			break;
		if (is_operator(line[i]))
		{
			new_token = get_operator_token(line, &i);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
		}
		else if (is_quote(line[i]))
		{
			word = get_quoted_word(line, &i, line[i]);
			if (!word)
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(TOKEN_WORD, word);
			free(word);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
		}
		else
		{
			word = get_word(line, &i);
			if (!word)
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(TOKEN_WORD, word);
			free(word);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
		}
	}
	return (tokens);
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
