/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:41 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:24:04 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_quote(char c, char *quote, int *i)
{
	if (!*quote)
		*quote = c;
	else
		*quote = 0;
	(*i)++;
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	quote;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) * 200 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (!quote || str[i] == quote))
			update_quote(str[i], &quote, &i);
		else if (str[i] == '$' && quote != '\'')
			handle_dollar(str, &i, &j, env);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}


bool	check_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote == 0);
}

char	*process_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			skip_quote(&quote, &i);
		else if (str[i] == quote)
			skip_quote(&quote, &i);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*cur_cmd;
	t_token	*cur_tok;

	cmds = create_cmd();
	if (!tokens || !cmds)
		return (NULL);
	cur_cmd = cmds;
	cur_tok = tokens;
	while (cur_tok)
	{
		if (cur_tok->type == TOKEN_WORD)
			handle_word(cur_cmd, cur_tok, env, cmds);
		else if (cur_tok->type == TOKEN_PIPE)
			handle_pipe(&cur_cmd, cmds);
		else if (is_redir(cur_tok->type))
			handle_redir(cur_cmd, &cur_tok, env, cmds);
		cur_tok = cur_tok->next;
	}
	return (cmds);
}
