/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 20:20:07 by mmorente          #+#    #+#             */
/*   Updated: 2026/03/19 21:26:09 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_word(t_cmd *cmd, t_token *tok, t_env *env, t_cmd *cmds)
{
	char	*expanded;

	expanded = expand_variables(tok->value, env);
	if (!expanded)
		return (free_commands(cmds), 0);
	if (!add_arg(cmd, expanded))
		return (free(expanded), free_commands(cmds), 0);
	free(expanded);
	return (1);
}

int	is_redir(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

int	handle_redir(t_cmd *cmd, t_token **tok, t_env *env, t_cmd *cmds)
{
	char			*expanded;
	t_token_type	type;

	if (!(*tok)->next || (*tok)->next->type != TOKEN_WORD)
		return (free_commands(cmds), 0);
	type = (*tok)->type;
	*tok = (*tok)->next;
	expanded = expand_variables((*tok)->value, env);
	if (!expanded)
		return (free_commands(cmds), 0);
	if (!add_redir(cmd, type - 2, expanded))
		return (free(expanded), free_commands(cmds), 0);
	free(expanded);
	return (1);
}

void	toggle_quote_state(char *quote, char c, int *i)
{
	if (!*quote)
		*quote = c;
	else
		*quote = 0;
	(*i)++;
}
