/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:01:32 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/03 19:39:47 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->exported = true;
	node->next = NULL;
	return (node);
}

bool	split_env_string(char *env_str, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (false);
	key_len = equal_sign - env_str;
	*key = ft_substr(env_str, 0, key_len);
	if (!*key)
		return (false);
	*value = ft_strdup(equal_sign + 1);
	if (!*value)
	{
		free(*key);
		return (false);
	}
	return (true);
}

void	add_env_to_list(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

bool	add_one_env(char *envp, t_env **env_list)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	if (!split_env_string(envp, &key, &value))
		return (true);
	new_node = create_env_node(key, value);
	free(key);
	free(value);
	if (!new_node)
		return (false);
	add_env_to_list(env_list, new_node);
	return (true);
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		if (!add_one_env(envp[i], &env_list))
		{
			free_env(env_list);
			return (NULL);
		}
		i++;
	}
	return (env_list);
}

