/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 19:24:37 by mmorente          #+#    #+#             */
/*   Updated: 2026/03/03 19:38:50 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->key, key, key_len + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

bool	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;
	char	*new_value;

	if (!key)
		return (false);
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			new_value = ft_strdup(value);
			if (!new_value)
				return (false);
			free(current->value);
			current->value = new_value;
			return (true);
		}
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (false);
	add_env_to_list(env, new_node);
	return (true);
}

bool	unset_env_value(t_env **env, char *key)
{
	t_env	**cur;
	t_env	*tmp;

	if (!env || !*env)
		return (false);
	cur = env;
	while (*cur)
	{
		if (ft_strncmp((*cur)->key, key, ft_strlen(key) + 1) == 0)
		{
			tmp = *cur;
			*cur = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return (true);
		}
		cur = &(*cur)->next;
	}
	return (false);
}
