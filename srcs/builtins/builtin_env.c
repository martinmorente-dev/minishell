/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:56:00 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/10 19:01:39 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	builtin_env(t_env *env, char **args)
{
	if (args[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (1);
	}
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
