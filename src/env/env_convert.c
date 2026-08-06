/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 20:38:45 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 17:40:41 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_envp_array(t_env *head)
{
	size_t	n;
	char	**arr;
	size_t	i;
	char	*pair;

	n = env_count(head);
	arr = (char **)ft_calloc(n + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (head)
	{
		if (head->value)
			pair = ft_strjoin3(head->key, "=", head->value);
		else
			pair = ft_strjoin3(head->key, "=", "");
		if (!pair)
			break ;
		arr[i++] = pair;
		head = head->next;
	}
	arr[i] = NULL;
	return (arr);
}
