/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:36 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:22:18 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar(char *str, int *i, int *j, t_env *env)
{
	int		start;
	char	*var_name;
	char	*value;

	start = *i + 1;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	value = get_env_value(env, var_name);
	if (value)
	{
		ft_strlcpy(&str[*j], value, 32);
		*j += ft_strlen(value);
		free(value);
	}
	else
		*j += 0;
	free(var_name);
}


