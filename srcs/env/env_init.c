/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:01:32 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/08 15:38:43 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static t_env	*create_env_node(char *key, char *value)
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
	node->next = NULL;
	return (node);
}

static bool	split_env_string(char *env_str, char **key, char **value)
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

static void	add_env_to_list(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		if (!split_env_string(envp[i], &key, &value))
		{
			i++;
			continue;
		}
		new_node = create_env_node(key, value);
		free(key);
		free(value);
		if (!new_node)
		{
			free_env(env_list);
			return (NULL);
		}
		add_env_to_list(&env_list, new_node);
		i++;
	}
	return (env_list);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

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
	char	*new_value;
	t_env	*new_node;
	size_t	key_len;

	key_len = ft_strlen(key);
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0)
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
	t_env	*current;
	t_env	*prev;
	size_t	key_len;

	key_len = ft_strlen(key);
	if (!*env)
		return (false);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (true);
		}
		prev = current;
		current = current->next;
	}
	return (false);
}
