/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 19:53:07 by mmorente          #+#    #+#             */
/*   Updated: 2026/03/19 21:40:14 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_redir(char c, t_token_type *type, char *op)
{
	int	len;

	len = 0;
	if (c == '<')
	{
		*type = TOKEN_REDIR_IN;
		op[len++] = '<';
	}
	else if (c == '>')
	{
		*type = TOKEN_REDIR_OUT;
		op[len++] = '>';
	}
	return (len);
}


int	detect_redir_type(char c, char next, t_token_type *type, char *op)
{
	int	len;

	len = 0;

	if (c == '<' && next == '<')
	{
		*type = TOKEN_HEREDOC;
		op[len++] = '<';
		op[len++] = '<';
	}
	else if (c == '>' && next == '>')
	{
		*type = TOKEN_REDIR_APPEND;
		op[len++] = '>';
		op[len++] = '>';
	}
	else
		len = simple_redir(c, type, op);
	return (len);
}
