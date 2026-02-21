/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:21:15 by mmorente          #+#    #+#             */
/*   Updated: 2026/02/17 19:44:08 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	show_exported_env(t_env *env)
{
	while (env)
	{
		if (env->exported == true && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

void	set_env_to_export(t_env *env, char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		 if (ft_strncmp(env->key, key, key_len) == 0 && 
            env->key[key_len] == '\0')
			env->exported = true;
		env = env->next;
	}
}

int	export_and_set(char *arg, t_env **env)
{
	char	**args_splited;

	args_splited = ft_split(arg, '=');
	if (args_splited[1])
	{
		set_env_value(env, args_splited[0], args_splited[1]);
		set_env_to_export(*env, args_splited[0]);
	}
	else
	{
		set_env_value(env, arg, NULL);
		set_env_to_export(*env, arg);
	}
	ft_free_double_ptr((void **) args_splited);
	return (0);
}



int	builtin_export(char **args, t_env **env)
{
	if (!args[1])
		return (show_exported_env(*env));
	else
		return (export_and_set(args[1], env));
	return (0);
}


