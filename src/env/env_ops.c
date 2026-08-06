/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 04:03:02 by rmunoz-g          #+#    #+#             */
/*   Updated: 2026/03/08 18:40:57 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_find_node(t_env *head, const char *key)
{
	while (head)
	{
		if (ft_streq(head->key, key))
			return (head);
		head = head->next;
	}
	return (NULL);
}

char	*env_get(t_env *head, const char *key)
{
	t_env	*n;

	n = env_find_node(head, key);
	if (n)
		return (n->value);
	return (NULL);
}

static bool	env_update(t_env *n, const char *value, bool ovwr)
{
	char	*newv;

	if (!ovwr)
		return (true);
	if (value)
		newv = ft_strdup(value);
	else
		newv = NULL;
	if (value && !newv)
		return (false);
	free(n->value);
	n->value = newv;
	return (true);
}

bool	env_set(t_env **head, const char *key, const char *value, bool ovwr)
{
	t_env	*n;
	t_env	*node;

	if (!head || !key)
		return (false);
	n = env_find_node(*head, key);
	if (n)
		return (env_update(n, value, ovwr));
	node = env_new(key, value);
	if (!node)
		return (false);
	node->next = *head;
	*head = node;
	return (true);
}

bool	env_unset(t_env **head, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!head || !*head || !key)
		return (false);
	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (ft_streq(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (true);
		}
		prev = cur;
		cur = cur->next;
	}
	return (false);
}
