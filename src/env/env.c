/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:24:02 by jsalado-          #+#    #+#             */
/*   Updated: 2026/03/07 20:46:00 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new(const char *key, const char *value)
{
	t_env	*n;

	n = (t_env *)malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	n->key = ft_strdup(key);
	if (value)
		n->value = ft_strdup(value);
	else
		n->value = NULL;
	n->next = NULL;
	if (!n->key || (value && !n->value))
	{
		free(n->key);
		free(n->value);
		free(n);
		return (NULL);
	}
	return (n);
}

static t_env	*parse_env_entry(const char *env_str)
{
	char	*eq;
	char	*key;
	char	*val;
	t_env	*node;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (NULL);
	key = ft_strndup(env_str, (size_t)(eq - env_str));
	val = ft_strdup(eq + 1);
	node = env_new(key, val);
	free(key);
	free(val);
	return (node);
}

t_env	*env_create_from_environ(char **environ)
{
	t_env	*head;
	t_env	**tail;
	size_t	i;
	t_env	*node;

	head = NULL;
	tail = &head;
	i = 0;
	while (environ && environ[i])
	{
		node = parse_env_entry(environ[i]);
		if (!node)
		{
			++i;
			continue ;
		}
		*tail = node;
		tail = &node->next;
		++i;
	}
	return (head);
}

void	env_destroy(t_env **head)
{
	t_env	*cur;
	t_env	*n;

	if (!head)
		return ;
	cur = *head;
	while (cur)
	{
		n = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = n;
	}
	*head = NULL;
}

size_t	env_count(t_env *head)
{
	size_t	n;

	n = 0;
	while (head)
	{
		++n;
		head = head->next;
	}
	return (n);
}
